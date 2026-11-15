||| BFS con mutación interna vía referencias lineales (idris2-ref1).
||| Interfaz pura (igual que BFSPuro): bfs : List (List Nat) -> Nat ->
||| List (Maybe Nat); por dentro dist y la cola viven en Ref1
||| (read1/write1 in-place) confinadas por run1.
module BFSRef1

import Data.Linear.Ref1
import Data.Linear.Token
import Data.SortedMap
import Cola

--%default partial
--%default total

enumerate : Nat -> List (List Nat) -> List (Nat, List Nat)
enumerate i [] = []
enumerate i (vs :: rest) = (i, vs) :: enumerate (i + 1) rest

indices : Nat -> List Nat
indices m = go 0
  where
    go : Nat -> List Nat
    go i = if i >= m then [] else i :: go (i + 1)

-- Contagiar vecinos: marca AL ENCOLAR, con write1 in-place en dist y q.
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

loop : (g : SortedMap Nat (List Nat))
    -> (dist : Ref s (SortedMap Nat Nat))
    -> (q : Ref s (Queue Nat)) -> F1' s
loop g dist q t =
  let dq # t := read1 q t
   in case dequeue dq of
        Nothing => () # t
        Just (u, q1) =>
          let _ # t := write1 q q1 t
              d # t := read1 dist t
           in case (lookup u d, lookup u g) of
                (Just du, Just vs) =>
                  let _ # t := infect dist q du vs t in loop g dist q t
                _ => loop g dist q t   -- inalcanzable: u salió de la cola

bfs1 : (g : SortedMap Nat (List Nat)) -> (n, origen : Nat) -> F1 s (List (Maybe Nat))
bfs1 g n origen t =
  let dist # t := ref1 (insert origen 0 empty) t
      q    # t := ref1 (enqueue origen empty) t
      _    # t := loop g dist q t
      d    # t := read1 dist t
   in map (\i => lookup i d) (indices n) # t

export
bfs : List (List Nat) -> Nat -> List (Maybe Nat)
bfs adj origen = run1 (bfs1 (fromList (enumerate 0 adj)) (length adj) origen)
