using mochilume_api.DTOs;
using mochilume_api.Models;
using System.Threading.Tasks;

namespace mochilume_api.Services.Interfaces
{
    public interface IAuthService
    {
        Task<ServiceResponse<bool>> RegisterAsync(PlayerAuthDto dto);
        Task<ServiceResponse<bool>> LoginAsync(PlayerAuthDto dto);
    }
}