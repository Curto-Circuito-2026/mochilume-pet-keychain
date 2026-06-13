using mochilume_api.DTOs;
using mochilume_api.Models;
using System.Threading.Tasks;

namespace mochilume_api.Services.Interfaces
{
    public interface ISaveService
    {
        Task<ServiceResponse<PlayerDataDto>> DownloadSaveAsync(string username);
        Task<ServiceResponse<bool>> UploadSaveAsync(PlayerDataDto dto);
    }
}