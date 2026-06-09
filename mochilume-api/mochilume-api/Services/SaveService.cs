using mochilume_api.DTOs;
using mochilume_api.Models;
using mochilume_api.Repositories.Interfaces;
using mochilume_api.Services.Interfaces;
using System.Linq;
using System.Threading.Tasks;

namespace mochilume_api.Services
{
    public class SaveService : ISaveService
    {
        private readonly IPlayerRepository _playerRepository;
        private readonly IPetRepository _petRepository;

        public SaveService(IPlayerRepository playerRepository, IPetRepository petRepository)
        {
            _playerRepository = playerRepository;
            _petRepository = petRepository;
        }

        public async Task<ServiceResponse<PlayerDataDto>> DownloadSaveAsync(string username)
        {
            var user = await _playerRepository.GetPlayerByUsernameAsync(username);
            if (user == null)
            {
                return ServiceResponse<PlayerDataDto>.Error("Usuário não encontrado.");
            }

            var highestIdPet = user.Pets
                .OrderByDescending(p => p.Id)
                .FirstOrDefault();

            var petsList = new List<PetDataDto>();
            if (highestIdPet != null)
            {
                petsList.Add(new PetDataDto
                {
                    Id = highestIdPet.Id,
                    Name = highestIdPet.Name,
                    Level = highestIdPet.Level,
                    Xp = highestIdPet.Xp,
                    Species = highestIdPet.Species,
                    IsActive = highestIdPet.IsActive
                });
            }

            var dto = new PlayerDataDto
            {
                Id = user.Id,
                UserName = user.UserName,
                Steps = user.Steps,
                Pets = petsList 
            };

            return ServiceResponse<PlayerDataDto>.Success(dto);
        }

        public async Task<ServiceResponse<bool>> UploadSaveAsync(PlayerDataDto dto)
        {
            var user = await _playerRepository.GetPlayerByUsernameAsync(dto.UserName);
            if (user == null)
            {
                return ServiceResponse<bool>.Error("Usuário não encontrado.");
            }

            user.Steps = dto.Steps;

            foreach (var petDto in dto.Pets)
            {
                if (petDto.Id > 0)
                {
                    var existingPet = user.Pets.FirstOrDefault(p => p.Id == petDto.Id);
                    if (existingPet != null)
                    {
                        existingPet.Name = petDto.Name;
                        existingPet.Level = petDto.Level;
                        existingPet.Xp = petDto.Xp;
                        existingPet.Species = petDto.Species;
                        existingPet.IsActive = petDto.IsActive;
                        await _petRepository.UpdateAsync(existingPet);
                    }
                }
                else
                {
                    var novoPet = new PetData
                    {
                        Name = petDto.Name,
                        Level = petDto.Level,
                        Xp = petDto.Xp,
                        Species = petDto.Species,
                        IsActive = petDto.IsActive,
                        PlayerId = user.Id
                    };
                    await _petRepository.AddAsync(novoPet);
                }
            }

            //var petIdsFromDto = dto.Pets.Select(p => p.Id).ToList();
            //var petsToRemove = user.Pets.Where(p => !petIdsFromDto.Contains(p.Id)).ToList();
            //foreach (var petToRemove in petsToRemove)
            //{
            //    await _petRepository.DeleteAsync(petToRemove.Id);
            //}

            await _playerRepository.UpdateAsync(user);

            return ServiceResponse<bool>.Success(true, "Save do jogador atualizado com sucesso!");
        }
    }
}