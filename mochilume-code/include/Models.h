#ifndef MODELS_H
#define MODELS_H

#include <Arduino.h>

// --- LIMITES DE MEMÓRIA (Ajuste conforme o game design) ---
#define MAX_USERNAME_LEN  32
#define MAX_PASSWORD_LEN  64
#define MAX_PET_NAME_LEN  24
#define MAX_PETS_INVENTORY 6  // Limite de pets que o jogador pode carregar

// ==========================================
// 1. PlayerAuthDto (Usado em /api/Auth/register e /api/Auth/login)
// ==========================================
typedef struct {
    char userName[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
} PlayerAuthDto;

// ==========================================
// 2. PetDataDto (Componente do inventário)
// ==========================================
typedef struct {
    int id;
    char name[MAX_PET_NAME_LEN]; // nullable: true no Swagger
    int level;
    int xp;
    int species;
    bool isActive;
} PetDataDto;

// ==========================================
// 3. PlayerDataDto (Usado em /api/Save/upload e download)
// ==========================================
typedef struct {
    int id;
    char userName[MAX_USERNAME_LEN]; 
    int steps;
    
    PetDataDto pets[MAX_PETS_INVENTORY];
    int petsCount; 
} PlayerDataDto;

#endif