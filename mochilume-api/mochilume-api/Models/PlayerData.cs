using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using Microsoft.EntityFrameworkCore;

namespace mochilume_api.Models
{
    [Index(nameof(UserName), IsUnique = true)]
    public class PlayerData
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Required]
        [MaxLength(20)]
        public string UserName { get; set; }

        [Required]
        [MinLength(4)]
        public string Password { get; set; }

        public int Steps { get; set; }

        public ICollection<PetData> Pets { get; set; } = new List<PetData>();
    }
}