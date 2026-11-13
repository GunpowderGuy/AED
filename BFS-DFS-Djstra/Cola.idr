||| Cola FIFO pura hecha con dos stacks (listas).
||| enqueue O(1), dequeue O(1) amortizado. Sin monadas:
||| el estado de la cola se pasa como valor.
module Cola

%default total

export
data Queue : Type -> Type where
  MkQueue : (front : List a) -> (back : List a) -> Queue a
-- front: el más antiguo adelante (listo para salir)
-- back : el más nuevo adelante (recién encolado)

export
empty : Queue a
empty = MkQueue [] []

export
enqueue : a -> Queue a -> Queue a
enqueue x (MkQueue f b) = MkQueue f (x :: b)

export
dequeue : Queue a -> Maybe (a, Queue a)
dequeue (MkQueue [] []) = Nothing
dequeue (MkQueue [] b)  = case reverse b of
  []        => Nothing
  (x :: xs) => Just (x, MkQueue xs [])
dequeue (MkQueue (x :: xs) b) = Just (x, MkQueue xs b)
