using mochilume_api.Data;
using mochilume_api.Models;
using mochilume_api.Repositories.Interfaces;

namespace mochilume_api.Repositories
{
    public class PetRepository : Repository<PetData>, IPetRepository
    {
        public PetRepository(AppDbContext context) : base(context)
        {
        }
    }
}