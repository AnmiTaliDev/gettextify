using System;
using System.IO;
using System.Text;
using System.Collections.Generic;

namespace CsvToMo
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Usage: CsvToMo <input.csv> <output.mo> [delimiter]");
                Console.WriteLine("  input.csv: CSV file with columns: msgid,msgstr,comment");
                Console.WriteLine("  output.mo: Output MO file path");
                Console.WriteLine("  delimiter: CSV delimiter character (default: ',')");
                return;
            }

            string inputFile = args[0];
            string outputFile = args[1];
            char delimiter = args.Length > 2 ? args[2][0] : ',';

            try
            {
                if (!File.Exists(inputFile))
                {
                    Console.WriteLine($"Error: Input file '{inputFile}' not found.");
                    return;
                }

                Console.WriteLine($"Converting {inputFile} to {outputFile}...");

                // Parse CSV and create translation entries
                List<TranslationEntry> entries = Translator.ParseCsv(inputFile, delimiter);
                Console.WriteLine($"Found {entries.Count} translation entries.");

                // Create directory for output file if it doesn't exist
                string outputDir = Path.GetDirectoryName(outputFile);
                if (!string.IsNullOrEmpty(outputDir) && !Directory.Exists(outputDir))
                {
                    Directory.CreateDirectory(outputDir);
                    Console.WriteLine($"Created directory: {outputDir}");
                }

                // Create MO file
                Translator.CreateMoFile(entries, outputFile);
                Console.WriteLine($"MO file created successfully: {outputFile}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
                Console.WriteLine(ex.StackTrace);
            }
        }
    }
}