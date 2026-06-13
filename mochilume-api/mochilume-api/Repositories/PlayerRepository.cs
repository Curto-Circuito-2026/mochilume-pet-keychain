using Microsoft.EntityFrameworkCore;
using mochilume_api.Data;
using mochilume_api.Models;
using mochilume_api.Repositories.Interfaces;
using System.Threading.Tasks;

namespace mochilume_api.Repositories
{
    public class PlayerRepository : Repository<PlayerData>, IPlayerRepository
    {
        public PlayerRepository(AppDbContext context) : base(context)
        {
        }

        public async Task<PlayerData> GetPlayerWithPetsAsync(int id)
        {
            return await _context.Players
                .Include(p => p.Pets)
                .FirstOrDefaultAsync(p => p.Id == id);
        }

        public async Task<PlayerData> GetPlayerByUsernameAsync(string username)
        {
            return await _context.Players
                .Include(p => p.Pets)
                .FirstOrDefaultAsync(p => p.UserName == username);
        }
    }
}