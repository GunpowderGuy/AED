# bfs_variants

Tres formas de escribir BFS sobre el ciclo 0-1-2-3-0 (desde el nodo 0),
con el mismo invariante: cada vecino se marca como alcanzado AL ENCOLAR.

| Archivo | Enfoque | Salida |
|---|---|---|
| `bfs.cpp` | C++23: `std::vector` + `std::queue`, mutación directa | `0 1 2 1` |
| `BFSRef1.idr` | Idris 2 con idris2-ref1: mutación interna (refs lineales `Ref` confinadas con `run1`), interfaz pura | `[Just 0, Just 1, Just 2, Just 1]` |
| `BFSFun.idr` | Idris 2 completamente puro: `dist` y la cola viajan como valores | `[Just 0, Just 1, Just 2, Just 1]` |

`Cola.idr` es la cola FIFO pura (dos stacks) que comparten ambas
variantes Idris; `Main.idr` imprime las dos.

## Compilar y ejecutar

```bash
# C++23
g++ -std=c++23 -O2 -o bfs bfs.cpp
./bfs

# Idris 2
idris2 -p ref1 -p linear Main.idr -o main
./build/exec/main
```

Ojo: con `idris2` directo `-p linear` es obligatorio junto a `-p ref1`
(no resuelve dependencias transitivas; el síntoma de olvidarlo es
`Module Data.Linear not found`). Con pack (`pack install ref1`, o un
`.ipkg` + `pack run`) esto se maneja solo.

Otro detalle: en ref1 v0.7.0 el tipo es `Ref s a` (no `Ref1` como
muestran algunos snippets del README de ref1).
