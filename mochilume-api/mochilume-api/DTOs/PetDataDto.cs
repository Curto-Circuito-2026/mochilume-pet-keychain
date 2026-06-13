namespace mochilume_api.DTOs
{
    public class PetDataDto
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public int Level { get; set; }
        public int Xp { get; set; }
        public int Species { get; set; }
        public bool IsActive { get; set; }
    }
}