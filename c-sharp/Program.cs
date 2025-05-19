namespace brainfrick;

using brainfrick.interpreter;

class Program
{
    public static void Main(string[] args)
    {
        if (args.Length != 1)
        {
            Console.Error.WriteLine("ERROR: Wrong usage.");
            Console.Error.WriteLine("Usage: brainfrick <src_file>.");
            return;
        }

        FileStream src;
        try
        {
            src = File.Open(args[0], FileMode.Open, FileAccess.Read);
        }
        catch (Exception e)
        {
            switch (e)
            {
                case FileNotFoundException:
                    Console.Error.WriteLine("ERROR: Inputted file does not exist.");
                    break;
                default:
                    Console.Error.WriteLine("ERROR: Unknown error encountered.");
                    break;
            }
            return;
        }

        Interpreter i = new(src, 10000);
        try
        {
            i.Run();
        }
        catch (Exception e)
        {
            Console.Error.WriteLine(e.Message);
            return;
        }

        return;
    }
}