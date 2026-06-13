using mochilume_api.Models;
using System.Threading.Tasks;

namespace mochilume_api.Repositories.Interfaces
{
    public interface IPlayerRepository : IRepository<PlayerData>
    {
        Task<PlayerData> GetPlayerWithPetsAsync(int id);
        Task<PlayerData> GetPlayerByUsernameAsync(string username);
    }
}