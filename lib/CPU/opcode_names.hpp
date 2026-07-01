#pragma once
#include <array>

// Noms (mnémoniques) des opcodes, pour la trace/désassemblage.
// Les tokens "n8", "n16", "a8", "a16", "e8" sont remplacés par la valeur réelle
// de l'opérande au moment du désassemblage. nullptr = opcode non nommé.
extern const std::array<const char*, 256> OPCODE_NAMES;
extern const std::array<const char*, 256> CB_OPCODE_NAMES;
