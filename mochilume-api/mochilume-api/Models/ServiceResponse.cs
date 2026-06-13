namespace mochilume_api.Models
{
    public class ServiceResponse<T>
    {
        public bool HasError { get; set; }
        public string Message { get; set; }
        public T Model { get; set; }

        public static ServiceResponse<T> Success(T model, string message = "")
        {
            return new ServiceResponse<T> { HasError = false, Model = model, Message = message };
        }

        public static ServiceResponse<T> Error(string message)
        {
            return new ServiceResponse<T> { HasError = true, Message = message };
        }
    }
}