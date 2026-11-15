||| BFS completamente puro: sin IO, sin ST/STRef, sin ref1.
||| dist y la cola son valores persistentes que se pasan explícitamente
||| como argumentos y resultados. Mismo invariante: marcar AL ENCOLAR.
module BFSFun

import Data.SortedMap
import Cola

%default partial

enumerate : Nat -> List (List Nat) -> List (Nat, List Nat)
enumerate i [] = []
enumerate i (vs :: rest) = (i, vs) :: enumerate (i + 1) rest

indices : Nat -> List Nat
indices m = go 0
  where
    go : Nat -> List Nat
    go i = if i >= m then [] else i :: go (i + 1)

-- Contagiar vecinos: marca AL ENCOLAR. Devuelve (dist, cola) nuevos;
-- nada se muta: SortedMap y Queue son persistentes.
infect : SortedMap Nat Nat -> Nat -> Queue Nat -> List Nat
      -> (SortedMap Nat Nat, Queue Nat)
infect d du q [] = (d, q)
infect d du q (v :: vs) = case lookup v d of
  Nothing => infect (insert v (du + 1) d) du (enqueue v q) vs
  Just _  => infect d du q vs

loop : SortedMap Nat (List Nat) -> SortedMap Nat Nat -> Queue Nat
    -> SortedMap Nat Nat
loop g d q = case dequeue q of
  Nothing      => d
  Just (u, q1) => case (lookup u d, lookup u g) of
    (Just du, Just vs) => let (d2, q2) = infect d du q1 vs in loop g d2 q2
    _                  => loop g d q1   -- inalcanzable: u salió de la cola

export
bfs : List (List Nat) -> Nat -> List (Maybe Nat)
bfs adj origen = map (\i => lookup i dist) (indices (length adj))
  where
    g : SortedMap Nat (List Nat)
    g = fromList (enumerate 0 adj)

    dist : SortedMap Nat Nat
    dist = loop g (insert origen 0 empty) (enqueue origen empty)
