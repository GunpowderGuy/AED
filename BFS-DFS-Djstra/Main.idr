module Main

import BFSPuro

%default partial

-- Ciclo 0-1-2-3-0
g : List (List Nat)
g = [[1,3], [0,2], [1,3], [2,0]]

main : IO ()
main = printLn (bfs g 0)   -- bfs es pura: sin <-, sin IO
