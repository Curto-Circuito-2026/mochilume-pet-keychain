using System.Collections.Generic;

namespace mochilume_api.DTOs
{
    public class PlayerDataDto
    {
        public int Id { get; set; }
        public string UserName { get; set; }
        public int Steps { get; set; }

        public ICollection<PetDataDto> Pets { get; set; } = new List<PetDataDto>();
    }
}