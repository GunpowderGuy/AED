{-# LANGUAGE ScopedTypeVariables #-}

module Main (main) where

--import Data.Array (Array, bounds, (!))
--import Data.Array.ST (STUArray, newArray, readArray, writeArray, runSTUArray)
--import Data.Array.Unboxed (UArray)
import Control.Monad (forM_, when)
import Control.Monad.ST
import Data.STRef


import Data.Array (Array, bounds, (!), listArray)
import Data.Array.ST (STUArray, newArray, readArray, writeArray, runSTUArray)
import Data.Array.Unboxed (UArray, assocs)

-- ============================================================
-- Módulo Cola (oculto tras la exportación de Main)
-- ============================================================

-- El constructor no se exporta fuera de Main, así que solo se
-- puede usar a través de las funciones públicas.
newtype Queue s a = Queue (STRef s [a], STRef s [a])

newQueue :: ST s (Queue s a)
newQueue = do
  f <- newSTRef []
  b <- newSTRef []
  return (Queue (f, b))

enqueue :: a -> Queue s a -> ST s ()
enqueue x (Queue (_, b)) = modifySTRef' b (x:)

dequeue :: Queue s a -> ST s (Maybe a)
dequeue (Queue (f, b)) = do
  fs <- readSTRef f
  case fs of
    (x:xs) -> writeSTRef f xs >> return (Just x)
    [] -> do
      bs <- readSTRef b
      case reverse bs of
        []     -> return Nothing
        (x:xs) -> do
          writeSTRef f xs
          writeSTRef b []
          return (Just x)

-- ============================================================
-- Módulo BFS
-- ============================================================

-- bfs :: Array Int [Int] -> Int -> UArray Int Int
--
-- [Tipos]
--   Entrada:  Array Int [Int]  → array indexado por Int, cada celda es [Int]
--             Int              → vértice origen

-- en c++ seria similar a std::vector<std::vector<int>>

--   Salida:   UArray Int Int   → array indexado por Int, cada celda es Int
--
-- [Significado]
--   El Array Int [Int] representa el grafo como lista de adyacencia:
--     g ! v = lista de vecinos de v.
--   El Int de entrada es el vértice desde el que arranca la búsqueda.
--   El UArray Int Int de salida da, para cada vértice v:
--     dist ! v = cantidad mínima de aristas de origen a v,
--                o -1 si v no es alcanzable desde origen.

bfs :: Array Int [Int] -> Int -> UArray Int Int
bfs g origen = runSTUArray $ do
  let (lo, hi) = bounds g
  dist <- newArray (lo, hi) (-1) :: ST s (STUArray s Int Int)
  writeArray dist origen 0

  q <- newQueue
  enqueue origen q

  let loop = do
        mu <- dequeue q
        case mu of
          Nothing -> return ()
          Just u -> do
            du <- readArray dist u
            forM_ (g ! u) $ \v -> do
              dv <- readArray dist v
              when (dv == -1) $ do
                writeArray dist v (du + 1)
                enqueue v q
            loop

  loop
  return dist   -- runSTUArray congela sin copiar

-- ============================================================
-- Main (ejemplo de uso)
-- ============================================================

main :: IO ()
main = do
  -- Grafo de ejemplo: 0-1-2-3 con arista extra 0-3
  let g = listArray (0, 3) [[1,3], [0,2], [1,3], [2,0]]
      dist = bfs g 0
  print (assocs dist)  -- [(0,0), (1,1), (2,2), (3,1)]
