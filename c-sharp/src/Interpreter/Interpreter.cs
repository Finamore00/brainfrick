namespace brainfrick.interpreter;

using System.IO;

public class InvalidSyntaxException(string message) : Exception(message);

public class Interpreter(FileStream src, ulong cap)
{
    private readonly FileStream src = src;
    private readonly byte[] memory = new byte[cap];
    private readonly ulong MemCap = cap;
    private ulong MemPtr = 0;
    private ulong pc = 0;
    private Stack<ulong> st = new();

    public void Run()
    {
        char[] tokens = ['+', '-', '>', '<', ',', '.', '[', ']'];

        for (int c = src.ReadByte(); c != -1; c = src.ReadByte())
        {
            switch ((char)c)
            {
                case '+':
                    memory[MemPtr] += 1;
                    break;
                case '-':
                    memory[MemPtr] -= 1;
                    break;
                case '>':
                    MemPtr = MemPtr < MemCap + 1 ? MemPtr + 1 : MemPtr;
                    break;
                case '<':
                    MemPtr = MemPtr > 0 ? MemPtr - 1 : MemPtr;
                    break;
                case '.':
                    Console.Write((char)memory[MemPtr]);
                    break;
                case ',':
                    memory[MemPtr] = (byte)Console.ReadKey().KeyChar;
                    break;
                case '[':

                    if (memory[MemPtr] == 0)
                    {
                        //Look for matching ']'
                        ulong depth = 1;
                        for (c = src.ReadByte(); depth > 0 && c != -1; c = src.ReadByte())
                        {
                            if ((char)c == '[') depth += 1;
                            if ((char)c == ']') depth -= 1;

                            if (tokens.Contains((char)c)) pc++;
                        }

                        if (c == -1)
                        {
                            throw new InvalidSyntaxException("ERROR: Found '[' token without matching ']'.");
                        }
                    }
                    else
                    {
                        st.Push(pc);
                    }
                    break;
                case ']':
                    if (st.Count <= 0)
                    {
                        throw new InvalidSyntaxException("ERROR: Found ']' without matching '['");
                    }

                    if (memory[MemPtr] != 0)
                    {
                        ulong TgtPc = st.Peek();
                        while (pc > TgtPc)
                        {
                            src.Seek(-2, SeekOrigin.Current);
                            if (tokens.Contains((char)src.ReadByte())) pc--;
                        }
                    }
                    else
                    {
                        st.Pop();
                    }
                    break;
                default:
                    continue; //Everything else is dismissed as a comment
            }

            pc++;
        }
    }
}