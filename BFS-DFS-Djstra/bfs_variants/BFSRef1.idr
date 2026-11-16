||| BFS con mutación interna vía referencias lineales (idris2-ref1).
||| Interfaz pura (igual que BFSPuro): bfs : List (List Nat) -> Nat ->
||| List (Maybe Nat); por dentro dist y la cola viven en Ref
||| (read1/write1 in-place) confinadas por run1.
|||
||| TERMINACIÓN: el bucle es `forN fuel` con
||| fuel = S (sum (map length adj)) (aristas + 1, computable desde la
||| entrada), así que bfs es total por construcción: acaba en a lo sumo
||| fuel pasos, quiera o no la cola vaciarse.
|||
||| PREGUNTA ABIERTA (documentada a propósito, sin resolver aquí):
||| ¿basta ese fuel para que la cola termine de vaciarse? Dicho de otro
||| modo: si el bucle fuera infinito, ¿rompería de todos modos en
||| ≤ fuel iteraciones? El compilador no lo garantiza; si el fuel no
||| bastara, bfs devolvería distancias incompletas sin error. Razón de
||| una línea para creerlo (no verificada): la componente alcanzable
||| desde el origen es conexa, así que sus vértices ≤ sus aristas + 1
||| ≤ fuel, y con marcar AL ENCOLAR cada vértice se desencola a lo sumo
||| una vez.
module BFSRef1

import Data.Linear.Ref1
import Data.Linear.Token
import Data.SortedMap
import Cola

%default total

enumerate : Nat -> List (List Nat) -> List (Nat, List Nat)
enumerate i [] = []
enumerate i (vs :: rest) = (i, vs) :: enumerate (i + 1) rest

-- Total: la recursión es estructural sobre el contador.
indices : Nat -> List Nat
indices m = go m []
  where
    go : Nat -> List Nat -> List Nat
    go Z     acc = acc
    go (S k) acc = go k (k :: acc)

-- Contagiar vecinos: marca AL ENCOLAR, con write1 in-place en dist y q.
-- Total: recursión estructural sobre la lista de vecinos.
infect : (dist : Ref s (SortedMap Nat Nat)) -> (q : Ref s (Queue Nat))
      -> Nat -> List Nat -> F1' s
infect dist q du [] t = () # t
infect dist q du (v :: vs) t =
  let d # t := read1 dist t
   in case lookup v d of
        Just _  => infect dist q du vs t
        Nothing =>
          let _  # t := write1 dist (insert v (du + 1) d) t
              dq # t := read1 q t
              _  # t := write1 q (enqueue v dq) t
           in infect dist q du vs t

bfs1 : (g : SortedMap Nat (List Nat)) -> (fuel, n, origen : Nat) -> F1 s (List (Maybe Nat))
bfs1 g fuel n origen t =
  let dist # t := ref1 (insert origen 0 empty) t
      q    # t := ref1 (enqueue origen empty) t

      -- El bucle completo en un solo sitio, como un
      -- `for (i = 0; i < fuel; i++) { ... }` de C++: el cuerpo (paso) va
      -- junto al forN. Cola vacía = no-op, que es el "break".
      -- Total: paso no recursa y forN es estructural en el fuel.
      paso : F1' s
      paso = \t1 =>
        let dq # t1 := read1 q t1
         in case dequeue dq of
              Nothing => () # t1
              Just (u, q1) =>
                let _ # t1 := write1 q q1 t1
                    d # t1 := read1 dist t1
                 in case (lookup u d, lookup u g) of
                      (Just du, Just vs) => infect dist q du vs t1
                      _ => () # t1   -- inalcanzable: u salió de la cola

      _    # t := forN fuel paso t
      d    # t := read1 dist t
   in map (\i => lookup i d) (indices n) # t

-- Versión anterior del bucle: recursión explícita sobre el fuel en lugar
-- de forN (forN ES exactamente esta recursión). Se conserva comentada
-- para comparar las dos formas.
-- loop : Nat -> (g : SortedMap Nat (List Nat))
--     -> (dist : Ref s (SortedMap Nat Nat))
--     -> (q : Ref s (Queue Nat)) -> F1' s
-- loop Z _ _ _ t = () # t   -- fuel agotado: inalcanzable con la cota correcta
-- loop (S k) g dist q t =
--   let dq # t := read1 q t
--    in case dequeue dq of
--         Nothing => () # t
--         Just (u, q1) =>
--           let _ # t := write1 q q1 t
--               d # t := read1 dist t
--            in case (lookup u d, lookup u g) of
--                 (Just du, Just vs) =>
--                   let _ # t := infect dist q du vs t in loop k g dist q t
--                 _ => loop k g dist q t   -- inalcanzable: u salió de la cola

export
bfs : List (List Nat) -> Nat -> List (Maybe Nat)
bfs adj origen =
  run1 (bfs1 (fromList (enumerate 0 adj)) (S (sum (map length adj))) (length adj) origen)
