using mochilume_api.DTOs;
using mochilume_api.Models;
using mochilume_api.Repositories.Interfaces;
using mochilume_api.Services.Interfaces;
using System.Threading.Tasks;
using BCrypt.Net;

namespace mochilume_api.Services
{
    public class AuthService : IAuthService
    {
        private readonly IPlayerRepository _playerRepository;

        public AuthService(IPlayerRepository playerRepository)
        {
            _playerRepository = playerRepository;
        }

        public async Task<ServiceResponse<bool>> RegisterAsync(PlayerAuthDto dto)
        {
            var existingUser = await _playerRepository.GetPlayerByUsernameAsync(dto.UserName);
            //if (existingUser != null)
            //{
            //    return ServiceResponse<bool>.Error("Nome de usuário já está em uso.");
            //}

            var newPlayer = new PlayerData
            {
                UserName = dto.UserName,
                Password = BCrypt.Net.BCrypt.HashPassword(dto.Password)
            };

            await _playerRepository.AddAsync(newPlayer);

            return ServiceResponse<bool>.Success(true, "Usuário registrado com sucesso!");
        }

        public async Task<ServiceResponse<bool>> LoginAsync(PlayerAuthDto dto)
        {
            var user = await _playerRepository.GetPlayerByUsernameAsync(dto.UserName);

            if (user == null)
            {
                var registerResult = await RegisterAsync(dto);
                user = await _playerRepository.GetPlayerByUsernameAsync(dto.UserName);

            }

            bool isPasswordValid = BCrypt.Net.BCrypt.Verify(dto.Password, user.Password);

            if (!isPasswordValid)
            {
                return ServiceResponse<bool>.Error("Senha incorreta.");
            }

            return ServiceResponse<bool>.Success(true, "Login bem sucedido.");
        }
    }
}