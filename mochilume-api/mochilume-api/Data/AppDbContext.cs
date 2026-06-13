using Microsoft.EntityFrameworkCore;
using mochilume_api.Models;

namespace mochilume_api.Data
{
    public class AppDbContext : DbContext
    {
        public AppDbContext(DbContextOptions<AppDbContext> options) : base(options)
        {
        }

        public DbSet<PlayerData> Players { get; set; }
        public DbSet<PetData> Pets { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);

            modelBuilder.Entity<PlayerData>()
                .HasMany(p => p.Pets)
                .WithOne(pe => pe.Player)
                .HasForeignKey(pe => pe.PlayerId)
                .OnDelete(DeleteBehavior.Cascade);
        }
    }
}