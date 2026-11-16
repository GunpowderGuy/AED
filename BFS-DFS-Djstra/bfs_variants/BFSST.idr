||| BFS con Control.Monad.ST (la sección "The ST Monad" del README de
||| ref1): refs mutables (STRef) + do-notation, para que el código se lea
||| como el C++ de bfs.cpp. ST es una mónada, así que los for son `for_`
||| (traverse_ con los argumentos volteados, el forM_ de Haskell) con el
||| cuerpo inline, como en C++: ningún bucle va en función aparte.
||| Interfaz pura, igual que las demás: bfsST : List (List Nat) -> Nat ->
||| List (Maybe Nat). Total con el mismo fuel: cada nodo se encola
||| ≤ 1 vez (marcar AL ENCOLAR), así que S (sum (map length adj)) acota
||| los pasos.
module BFSST

import Control.Monad.ST
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

-- Misma forma que el main de bfs.cpp: un único procedimiento con los
-- bucles y sus cuerpos inline (nada de cuerpos en funciones aparte).
-- El while de C++ es aquí un for_ con fuel (cota de pasos); el for de
-- vecinos, un for_ sobre g[u].
bfsST1 : (g : SortedMap Nat (List Nat)) -> (fuel, n, origen : Nat) -> ST s (List (Maybe Nat))
bfsST1 g fuel n origen = do
  dist <- newSTRef (insert origen 0 empty)  -- dist[origen] = 0
  q    <- newSTRef (enqueue origen empty)   -- q.push(origen)
  for_ (indices fuel) $ \_ => do            -- bucle principal (fuel pasos)
    dq <- readSTRef q
    case dequeue dq of
      Nothing => pure ()                    -- cola vacía: no-op (el "break")
      Just (u, q1) => do                    -- u = q.front(); q.pop()
        writeSTRef q q1
        d <- readSTRef dist
        case (lookup u d, lookup u g) of
          (Just du, Just vs) =>
            for_ vs $ \v => do              -- for (v : g[u])
              d2 <- readSTRef dist
              case lookup v d2 of
                Just _  => pure ()          -- ya marcado
                Nothing => do               -- marcar AL ENCOLAR
                  writeSTRef dist (insert v (du + 1) d2)  -- dist[v] = du+1
                  dq2 <- readSTRef q
                  writeSTRef q (enqueue v dq2)            -- q.push(v)
          _ => pure ()
  d <- readSTRef dist
  pure (map (\i => lookup i d) (indices n)) -- imprimir dist por índice

export
bfsST : List (List Nat) -> Nat -> List (Maybe Nat)
bfsST adj origen =
  runST (bfsST1 (fromList (enumerate 0 adj)) (S (sum (map length adj))) (length adj) origen)
