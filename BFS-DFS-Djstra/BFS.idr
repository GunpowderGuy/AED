||| BFS: mismo algoritmo que el Haskell (mismas oleadas, mismo
||| invariante: marcar AL ENCOLAR). La mutación en el lugar vive
||| solo en dist (IOArray, O(1) por acceso); la cola es pura (Cola.idr).
module BFS

import Data.IOArray
import Cola

%default partial

fill : IOArray (List Nat) -> Nat -> List (List Nat) -> IO ()
fill g i [] = pure ()
fill g i (vs :: rest) = do
  ignore (writeArray g (cast i) vs)
  fill g (i + 1) rest

-- Intentar contagiar a cada vecino: solo los no contagiados se
-- marcan (AL ENCOLAR) y entran a la cola. Devuelve la cola avanzada.
infect : IOArray Nat -> Nat -> Queue Nat -> List Nat -> IO (Queue Nat)
infect dist du q [] = pure q
infect dist du q (v :: vs) = do
  dv <- readArray dist (cast v)
  if dv == Nothing
    then do
      ignore (writeArray dist (cast v) (du + 1))   -- marcar AL ENCOLAR
      infect dist du (enqueue v q) vs
    else infect dist du q vs

loop : IOArray (List Nat) -> IOArray Nat -> Queue Nat -> IO ()
loop g dist q = case dequeue q of
  Nothing      => pure ()
  Just (u, q1) => do
    Just du <- readArray dist (cast u) | Nothing => pure ()  -- inalcanzable
    Just vs <- readArray g (cast u)    | Nothing => pure ()
    q2 <- infect dist du q1 vs
    loop g dist q2

-- "Congelar" al final: copia O(n), lo mismo que assocs en el Haskell.
freeze : IOArray e -> IO (List (Maybe e))
freeze arr = go 0 []
  where
    go : Int -> List (Maybe e) -> IO (List (Maybe e))
    go i acc =
      if i >= max arr
        then pure (reverse acc)
        else do
          x <- readArray arr i
          go (i + 1) (x :: acc)

export
bfs : List (List Nat) -> Nat -> IO (List (Maybe Nat))
bfs adj origen = do
  let n = length adj
  g    <- newArray (cast n)
  fill g 0 adj
  dist <- newArray (cast n)   -- nace todo Nothing = "no contagiado"
  ignore (writeArray dist (cast origen) 0)
  loop g dist (enqueue origen empty)
  freeze dist
