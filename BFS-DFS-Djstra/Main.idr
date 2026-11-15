module Main

--import BFSPuro
import BFS

%default partial

-- Ciclo 0-1-2-3-0
g : List (List Nat)
g = [[1,3], [0,2], [1,3], [2,0]]

main : IO ()
main = do
  dist <- bfs g 0   -- versión IO (IOArray): hay que sacarla con <-
  printLn dist
