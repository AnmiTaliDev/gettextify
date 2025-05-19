using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Linq;

namespace CsvToMo
{
    public class TranslationEntry
    {
        public string MsgId { get; set; }
        public string MsgStr { get; set; }
        public string Comment { get; set; }

        public TranslationEntry(string msgId, string msgStr, string comment)
        {
            MsgId = msgId;
            MsgStr = msgStr;
            Comment = comment;
        }
    }

    public static class Translator
    {
        // MO file format constants
        private const uint MO_MAGIC = 0x950412de;
        private const uint MO_FORMAT_REVISION = 0;
        private const int MO_HEADER_SIZE = 28;

        public static List<TranslationEntry> ParseCsv(string filePath, char delimiter)
        {
            List<TranslationEntry> entries = new List<TranslationEntry>();
            string[] lines = File.ReadAllLines(filePath);

            if (lines.Length == 0)
            {
                throw new Exception("CSV file is empty");
            }

            // Verify header and find column indices
            string[] header = lines[0].Split(delimiter);
            int msgidIndex = Array.IndexOf(header, "msgid");
            int msgstrIndex = Array.IndexOf(header, "msgstr");
            int commentIndex = Array.IndexOf(header, "comment");

            if (msgidIndex == -1 || msgstrIndex == -1)
            {
                throw new Exception("CSV must have 'msgid' and 'msgstr' columns");
            }

            // Process data rows
            for (int i = 1; i < lines.Length; i++)
            {
                if (string.IsNullOrWhiteSpace(lines[i]))
                {
                    continue;
                }

                string[] fields = SplitCsvLine(lines[i], delimiter);
                
                if (fields.Length <= Math.Max(Math.Max(msgidIndex, msgstrIndex), commentIndex))
                {
                    Console.WriteLine($"Warning: Skipping line {i+1} due to insufficient fields");
                    continue;
                }

                string msgid = fields[msgidIndex].Trim();
                string msgstr = fields[msgstrIndex].Trim();
                string comment = commentIndex >= 0 && commentIndex < fields.Length ? fields[commentIndex].Trim() : "";

                // Remove quotes if present
                msgid = RemoveQuotes(msgid);
                msgstr = RemoveQuotes(msgstr);
                comment = RemoveQuotes(comment);

                entries.Add(new TranslationEntry(msgid, msgstr, comment));
            }

            return entries;
        }

        // Helper to handle quoted CSV fields correctly
        private static string[] SplitCsvLine(string line, char delimiter)
        {
            List<string> fields = new List<string>();
            bool inQuotes = false;
            StringBuilder field = new StringBuilder();

            for (int i = 0; i < line.Length; i++)
            {
                char c = line[i];
                
                if (c == '"')
                {
                    if (inQuotes && i < line.Length - 1 && line[i + 1] == '"')
                    {
                        // Escaped quote
                        field.Append('"');
                        i++;
                    }
                    else
                    {
                        // Toggle quote mode
                        inQuotes = !inQuotes;
                    }
                }
                else if (c == delimiter && !inQuotes)
                {
                    // End of field
                    fields.Add(field.ToString());
                    field.Clear();
                }
                else
                {
                    field.Append(c);
                }
            }
            
            // Add last field
            fields.Add(field.ToString());
            
            return fields.ToArray();
        }

        private static string RemoveQuotes(string value)
        {
            if (value.StartsWith("\"") && value.EndsWith("\"") && value.Length >= 2)
            {
                return value.Substring(1, value.Length - 2);
            }
            return value;
        }

        public static void CreateMoFile(List<TranslationEntry> entries, string outputPath)
        {
            // Sort entries by msgid for binary search
            // GNU gettext требует строгой сортировки по байтам, а не по строкам
            entries = entries.OrderBy(e => 
            {
                if (string.IsNullOrEmpty(e.MsgId))
                {
                    // Пустой msgid (заголовок) должен быть первым
                    return "";
                }
                return BitConverter.ToString(Encoding.UTF8.GetBytes(e.MsgId)).Replace("-", "");
            }).ToList();

            // Добавим пустую запись в начало, если её нет (требуется для совместимости с gettext)
            if (entries.Count == 0 || !string.IsNullOrEmpty(entries[0].MsgId))
            {
                entries.Insert(0, new TranslationEntry("", 
                    $"Project-Id-Version: csv-to-mo 1.0\n" +
                    $"Report-Msgid-Bugs-To: \n" +
                    $"POT-Creation-Date: {DateTime.Now:yyyy-MM-dd HH:mmzzz}\n" +
                    $"PO-Revision-Date: {DateTime.Now:yyyy-MM-dd HH:mmzzz}\n" +
                    $"Last-Translator: CSV to MO converter\n" +
                    $"Language-Team: \n" +
                    $"MIME-Version: 1.0\n" +
                    $"Content-Type: text/plain; charset=UTF-8\n" +
                    $"Content-Transfer-Encoding: 8bit\n",
                    ""));
            }

            using (FileStream fs = new FileStream(outputPath, FileMode.Create))
            using (BinaryWriter writer = new BinaryWriter(fs))
            {
                int numStrings = entries.Count;
                int stringsOffset = MO_HEADER_SIZE;
                int stringTableOffset = stringsOffset + numStrings * 8 * 2;  // 8 bytes per entry (length + offset), 2 tables

                // Write MO file header
                writer.Write(MO_MAGIC);                   // Magic number
                writer.Write(MO_FORMAT_REVISION);         // File format revision
                writer.Write((uint)numStrings);           // Number of strings
                writer.Write((uint)stringsOffset);        // Offset of table with original strings
                writer.Write((uint)(stringsOffset + numStrings * 8)); // Offset of table with translation strings
                writer.Write((uint)0);                    // Size of hashing table (unused)
                writer.Write((uint)stringTableOffset);    // Offset of hashing table (unused)

                // Prepare string data
                List<byte[]> originalStrings = new List<byte[]>();
                List<byte[]> translatedStrings = new List<byte[]>();
                
                foreach (var entry in entries)
                {
                    originalStrings.Add(Encoding.UTF8.GetBytes(entry.MsgId));
                    translatedStrings.Add(Encoding.UTF8.GetBytes(entry.MsgStr));
                }

                // Calculate and write string table (original strings)
                int currentOffset = stringTableOffset;
                for (int i = 0; i < numStrings; i++)
                {
                    byte[] data = originalStrings[i];
                    writer.Write((uint)data.Length);
                    writer.Write((uint)currentOffset);
                    currentOffset += data.Length + 1; // +1 for null terminator
                }

                // Calculate and write string table (translated strings)
                currentOffset = stringTableOffset;
                foreach (var data in originalStrings)
                {
                    currentOffset += data.Length + 1;
                }
                
                for (int i = 0; i < numStrings; i++)
                {
                    byte[] data = translatedStrings[i];
                    writer.Write((uint)data.Length);
                    writer.Write((uint)currentOffset);
                    currentOffset += data.Length + 1; // +1 for null terminator
                }

                // Write string data (original strings)
                for (int i = 0; i < numStrings; i++)
                {
                    writer.Write(originalStrings[i]);
                    writer.Write((byte)0); // null terminator
                }

                // Write string data (translated strings)
                for (int i = 0; i < numStrings; i++)
                {
                    writer.Write(translatedStrings[i]);
                    writer.Write((byte)0); // null terminator
                }
            }
        }
    }
}