||| Variante con frontera PURA: runST + STRef + SortedMap.
||| Mismo algoritmo y misma cola pura que BFS.idr; dist es un
||| SortedMap dentro de un STRef (lookup/insert O(log V)).
module BFSPuro

import Control.Monad.ST
import Data.SortedMap
import Cola

%default partial

bfsST : SortedMap Nat (List Nat) -> Nat -> ST s (SortedMap Nat Nat)
bfsST g origen = do
  dist <- newSTRef (insert origen 0 empty)
  loop g (enqueue origen empty) dist
  readSTRef dist
  where
    -- Contagiar vecinos: marcar AL ENCOLAR, como siempre.
    infect : Nat -> Queue Nat -> List Nat -> STRef s (SortedMap Nat Nat) -> ST s (Queue Nat)
    infect du q [] dist = pure q
    infect du q (v :: vs) dist = do
      d <- readSTRef dist
      case lookup v d of
        Nothing => do
          writeSTRef dist (insert v (du + 1) d)
          infect du (enqueue v q) vs dist
        Just _  => infect du q vs dist

    loop : SortedMap Nat (List Nat) -> Queue Nat -> STRef s (SortedMap Nat Nat) -> ST s ()
    loop g q dist = case dequeue q of
      Nothing      => pure ()
      Just (u, q1) => do
        d <- readSTRef dist
        case (lookup u d, lookup u g) of
          (Just du, Just vs) => do
            q2 <- infect du q1 vs dist
            loop g q2 dist
          _ => pure ()   -- inalcanzable: u salió de la cola

enumerate : Nat -> List (List Nat) -> List (Nat, List Nat)
enumerate i [] = []
enumerate i (vs :: rest) = (i, vs) :: enumerate (i + 1) rest

indices : Nat -> List Nat
indices m = go 0
  where
    go : Nat -> List Nat
    go i = if i >= m then [] else i :: go (i + 1)

export
bfs : List (List Nat) -> Nat -> List (Maybe Nat)
bfs adj origen =
  map (\i => lookup i dist) (indices (length adj))
  where
    dist : SortedMap Nat Nat
    dist = runST (bfsST (fromList (enumerate 0 adj)) origen)
