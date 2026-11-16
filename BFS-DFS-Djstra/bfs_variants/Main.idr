-- Compilar: idris2 -p ref1 -p linear Main.idr -o main
module Main

import BFSRef1
import BFSFun
import BFSST

-- Ciclo 0-1-2-3-0
g : List (List Nat)
g = [[1,3], [0,2], [1,3], [2,0]]

main : IO ()
main = do
  printLn (BFSRef1.bfs g 0)   -- mutación interna (ref1), interfaz pura
  printLn (BFSFun.bfs g 0)    -- completamente puro
  printLn (BFSST.bfsST g 0)   -- Control.Monad.ST: refs + do + for_
