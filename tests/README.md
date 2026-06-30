# Tests

Batteries de tests unitaires autonomes (ne dépendent **pas** de raylib).

## Lancer les tests

```bash
cmake -S tests -B build-tests
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

Ou lancer un binaire directement : `./build-tests/test_cpu`.

## Contenu

| Cible | Couvre |
|---|---|
| `test_sanity`     | `bitwise`, `Register`/`FlagRegister`/`Register16`, `Timer` (DIV/TIMA/TAC) |
| `test_fakememory` | routage MMIO `0xFF04-07`, `tick` → `IF.bit2`, helpers de test |
| `test_cpu`        | stack (`push`/`pop`), interruptions (`handle_interrupts`), opcodes ADC/LD |

## Notes

- `test_cpu` accède aux membres privés du CPU via `struct CpuTest`, déclaré
  `friend` dans `lib/CPU/CPU.hpp`.
- `test_cpu.cpp` fournit un `CPU::run_opcode` **stub** tant que la vraie
  implémentation n'existe pas. **À supprimer** une fois `CPU::run_opcode` écrit,
  sinon double définition au link.
- `fakememory.hpp` est une `Memory` factice réservée aux tests (64 Kio + timer
  intégré). Ne pas la linker dans `gbemu`.

## Échecs attendus

`test_cpu` comporte 4 assertions qui **échouent volontairement** : elles pointent
des bugs connus à corriger (double incrément de PC sur ADC, `Register` passé par
valeur dans `opcode_ld_n8`, lecture mémoire manquante dans `opcode_ld_A_n16`).
Elles passeront au vert une fois ces bugs corrigés.
