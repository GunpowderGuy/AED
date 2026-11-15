# AGENTS.md — bfs_variants

Tres implementaciones de BFS sobre el mismo grafo (ciclo 0-1-2-3-0,
desde el nodo 0). Todas deben producir las distancias `0 1 2 1`.

## Layout

- `bfs.cpp` — C++23: mutación directa (`vector` dist + `queue`).
- `BFSRef1.idr` — interfaz pura con mutación interna (idris2-ref1:
  referencias lineales `Ref` confinadas con `run1`).
- `BFSFun.idr` — completamente puro: `dist` y la cola se pasan como
  valores explícitos (sin IO, ST ni ref1).
- `Cola.idr` — cola FIFO pura (dos stacks); la usan ambas variantes Idris.
- `Main.idr` — demo: imprime el resultado de las dos variantes Idris.
- `bfsvariants.ipkg` — opcional; permite `pack build` / `pack run`.

## Compilar

```bash
g++ -std=c++23 -O2 -o bfs bfs.cpp
idris2 -p ref1 -p linear Main.idr -o main
```

## Trampas del entorno (importante)

- `idris2` directo NO resuelve dependencias transitivas: hay que pasar
  `-p ref1 -p linear`. Si falta `-p linear`, el error es
  `Module Data.Linear not found` (`Data.Linear` vive en el paquete `linear`,
  no en `ref1`).
- ref1 v0.7.0: el tipo se llama `Ref s a` (NO `Ref1`); la función de
  creación es `ref1`. El README de ref1 muestra una API más nueva:
  no fiarse de los nombres de los snippets.
- Paquetes gestionados con pack (`pack install ref1`). Si se compila vía
  pack, pack añade las dependencias transitivas automáticamente.
- Existe una copia manual de ref1-0.7.0 dentro del directorio del
  compilador de pack (`~/.local/state/pack/install/<hash>/idris2/
  idris2-0.8.0/ref1-0.7.0`). No tiene desinstalador oficial; si algo raro
  pasa con la resolución de paquetes, sospechar de ella primero.

## Convenciones

- Comentarios en español.
- Los `.idr` usan `%default partial` (el BFS con cola no es obviamente total).
- Invariante del BFS en este repo: marcar AL ENCOLAR.

## Verificación

Compilar y ejecutar; salidas esperadas:

- C++: `0 1 2 1`
- Idris (`Main.idr`): dos líneas `[Just 0, Just 1, Just 2, Just 1]`
