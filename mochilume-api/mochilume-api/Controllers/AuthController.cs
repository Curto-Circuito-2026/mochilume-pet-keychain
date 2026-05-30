using Microsoft.AspNetCore.Mvc;
using mochilume_api.DTOs;
using mochilume_api.Services.Interfaces;
using System.Threading.Tasks;

namespace mochilume_api.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class AuthController : ControllerBase
    {
        private readonly IAuthService _authService;

        public AuthController(IAuthService authService)
        {
            _authService = authService;
        }

        [HttpPost("register")]
        public async Task<IActionResult> Register([FromBody] PlayerAuthDto dto)
        {
            var response = await _authService.RegisterAsync(dto);
            if (response.HasError)
            {
                return BadRequest(response); 
            }

            return Ok(response); 
        }

        [HttpPost("login")]
        public async Task<IActionResult> Login([FromBody] PlayerAuthDto dto)
        {
            var response = await _authService.LoginAsync(dto);
            if (response.HasError)
            {
                return BadRequest(response);
            }

            return Ok(response);
        }
    }
}