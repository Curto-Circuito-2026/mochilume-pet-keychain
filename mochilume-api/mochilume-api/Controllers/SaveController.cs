using Microsoft.AspNetCore.Mvc;
using mochilume_api.DTOs;
using mochilume_api.Services.Interfaces;
using System.Threading.Tasks;

namespace mochilume_api.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class SaveController : ControllerBase
    {
        private readonly ISaveService _saveService;

        public SaveController(ISaveService saveService)
        {
            _saveService = saveService;
        }

        [HttpGet("{username}/download")]
        public async Task<IActionResult> DownloadSave(string username)
        {
            var response = await _saveService.DownloadSaveAsync(username);
            if (response.HasError)
            {
                return NotFound(response);
            }

            return Ok(response);
        }

        [HttpPut("upload")]
        public async Task<IActionResult> UploadSave([FromBody] PlayerDataDto dto)
        {
            var response = await _saveService.UploadSaveAsync(dto);
            if (response.HasError)
            {
                return BadRequest(response);
            }

            return Ok(response);
        }
    }
}