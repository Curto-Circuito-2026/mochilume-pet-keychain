namespace mochilume_api.Models
{
    public class PetData
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public int Level { get; set; }
        public int Xp { get; set; }
        public int Species { get; set; }

        public bool IsActive { get; set; } = false;

        public int PlayerId { get; set; }
        public PlayerData Player { get; set; }
    }
}