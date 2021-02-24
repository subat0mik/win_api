using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DotNetAsync
{
    class Program
    {
        static void Main(string[] args)
        {
            var task = DoReadAsync(@"\\c:\users\admin\desktop\largefile.dat");
            while (true)
            {
                if (task.Wait(100))
                    break;
                Console.Write(".");
            }
            Console.WriteLine("\nDone! Read: {0}", task.Result);
        }

        static async Task<int> DoReadAsync(string path)
        {
            using(var fs = new FileStream(path, FileMode.Open, FileAccess.Read))
            {
                var buffer = new byte[(int)fs.Length];
                int read = await fs.ReadAsync(buffer, 0, (int)fs.Length);
                return read;
            }
        }

        static Task<int> DoReadAsync2(string path)
        {
            var tcs = new TaskCompletionSource<int>();
            var fs = new FileStream(path, FileMode.Open, FileAccess.Read);
            var buffer = new byte[(int)fs.Length];
            fs.BeginRead(buffer, 0, (int)fs.Length, ar =>
            {
                tcs.SetResult(fs.EndRead(ar));
                fs.Close();
            }, null);

            return tcs.Task;
        }
    }
}
