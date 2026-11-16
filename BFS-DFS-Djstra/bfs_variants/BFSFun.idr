||| BFS completamente puro: sin IO, sin ST/STRef, sin ref1.
||| dist y la cola son valores persistentes que se pasan explícitamente
||| como argumentos y resultados. Mismo invariante: marcar AL ENCOLAR.
module BFSFun

import Data.SortedMap
import Cola

%default total

enumerate : Nat -> List (List Nat) -> List (Nat, List Nat)
enumerate i [] = []
enumerate i (vs :: rest) = (i, vs) :: enumerate (i + 1) rest

indices : Nat -> List Nat
indices m = go m 0
  where
    -- baja por el contador (estructural); antes: go (i+1), no estructural
    go : Nat -> Nat -> List Nat
    go Z     _ = []
    go (S k) i = i :: go k (S i)

-- Contagiar vecinos: marca AL ENCOLAR. Devuelve (dist, cola) nuevos;
-- nada se muta: SortedMap y Queue son persistentes.
infect : SortedMap Nat Nat -> Nat -> Queue Nat -> List Nat
      -> (SortedMap Nat Nat, Queue Nat)
infect d du q [] = (d, q)
infect d du q (v :: vs) = case lookup v d of
  Nothing => infect (insert v (du + 1) d) du (enqueue v q) vs
  Just _  => infect d du q vs

||| Bucle con fuel: como cada nodo se encola como mucho una vez
||| (invariante: marcar AL ENCOLAR), el número de pasos está acotado por
||| 1 + (entradas de adyacencia); con esa cota el caso Z nunca se da y la
||| recursión es estructural en el fuel.
loop : SortedMap Nat (List Nat) -> Nat -> SortedMap Nat Nat -> Queue Nat
    -> SortedMap Nat Nat
loop _ Z d _ = d   -- fuel agotado: inalcanzable con la cota correcta
loop g (S k) d q = case dequeue q of
  Nothing      => d
  Just (u, q1) => case (lookup u d, lookup u g) of
    (Just du, Just vs) => let (d2, q2) = infect d du q1 vs in loop g k d2 q2
    _                  => loop g k d q1   -- inalcanzable: u salió de la cola

export
bfs : List (List Nat) -> Nat -> List (Maybe Nat)
bfs adj origen = map (\i => lookup i dist) (indices (length adj))
  where
    g : SortedMap Nat (List Nat)
    g = fromList (enumerate 0 adj)

    fuel : Nat
    fuel = S (sum (map length adj))   -- cota segura de pasos (ver loop)

    dist : SortedMap Nat Nat
    dist = loop g fuel (insert origen 0 empty) (enqueue origen empty)
