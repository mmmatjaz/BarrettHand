using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Reflection;
using System.IO;

namespace BHlauncher
{
    class Setts
    {
        public string IP;
        public int RXport;
        public int TXport;
        public bool use280;
        public bool PPS;
        public bool use262;
        public int com;
        public string RTfile;
        public string RTpath;
        public string Sfile;
        public string Spath;

        public Setts()
        {
            IP = "127.0.0.1";
            RXport = 4444;
            TXport = 5555;
            use280 = true;
            PPS = true;
            use262 = true;
            com = 1;
            RTfile = "bhRTserver.exe";
            RTpath = Environment.CurrentDirectory;
            Sfile = "bhSserver.exe";
            Spath = Environment.CurrentDirectory;
        }

        public Setts(   string IP, int RX, int TX, 
                        bool use280, bool use262, bool PPS, int com,
                        string RTfn, string RTp, string Sfn, string Sp)
        {
            this.IP = IP;
            this.RXport = RX;
            this.TXport = TX;
            this.use280 = use280;
            this.use262 = use262;
            this.PPS = PPS;
            this.com = com;
            this.RTfile = RTfn;
            this.RTpath = RTp;
            this.RTfile = Sfn;
            this.RTpath = Sp;
        }

        public int Load()
        {
            if (!File.Exists("settings.xml"))
                return -2;

            XmlDocument doc = new XmlDocument();

            doc.Load(@"settings.xml");

            
            Type type = this.GetType();
            FieldInfo[] fields = type.GetFields();

            /*
            foreach (FieldInfo field in fields)
            {
                string fieldName = field.Name;
                Type fieldType = field.FieldType;
                //XmlNode nodica = doc.GetElementsByTagName(fieldName)[0];
                //XmlNode noddde= doc.GetElementsByTagName(fieldName)[0];
                XmlNode noddd = doc.GetElementsByTagName(fieldName)[0];
                Console.WriteLine(fieldName + "  " + noddd.ChildNodes[0].Value + "  " + fieldType.ToString());
                
                field.SetValue(this, Enum.Parse(fieldType, noddd.ChildNodes[0].Value));
                              
            }
            */
            this.IP = doc.GetElementsByTagName("IP")[0].ChildNodes[0].Value;
            this.RTfile = doc.GetElementsByTagName("RTfile")[0].ChildNodes[0].Value;
            this.RTpath = doc.GetElementsByTagName("RTpath")[0].ChildNodes[0].Value;
            this.Sfile = doc.GetElementsByTagName("Sfile")[0].ChildNodes[0].Value;
            this.Spath = doc.GetElementsByTagName("Spath")[0].ChildNodes[0].Value;
            this.RXport = int.Parse(doc.GetElementsByTagName("RXport")[0].ChildNodes[0].Value);
            this.TXport = int.Parse(doc.GetElementsByTagName("TXport")[0].ChildNodes[0].Value);
            this.use262 = bool.Parse(doc.GetElementsByTagName("use262")[0].ChildNodes[0].Value);
            this.use280 = bool.Parse(doc.GetElementsByTagName("use280")[0].ChildNodes[0].Value);
            this.PPS = bool.Parse(doc.GetElementsByTagName("PPS")[0].ChildNodes[0].Value);
            this.com = int.Parse(doc.GetElementsByTagName("com")[0].ChildNodes[0].Value);
            
            return 1;
        }

        public void Save()
        {
            //XmlTextWriter textWriter = new XmlTextWriter("settings.xml",null);
            //textWriter.WriteStartDocument();
            XmlDocument xmldoc = new XmlDocument();
            XmlNode xmlRoot, xmlNode;

            xmlRoot = xmldoc.CreateElement("settings");
            xmldoc.AppendChild(xmlRoot);

            Type type = this.GetType();
            FieldInfo[] fields = type.GetFields();

            foreach (FieldInfo field in fields)
            {
                xmlNode = xmldoc.CreateElement(field.Name);
                xmlRoot.AppendChild(xmlNode);
                var fieldValue = field.GetValue(this);
                xmlNode.InnerText = fieldValue.ToString();
            }

            xmldoc.Save("settings.xml");
        }

        public string getArgument()
        {
            string argument = "";
            //argument += this.filename + " ";
            argument += this.IP + " ";
            argument += this.RXport + " ";
            argument += this.TXport + " ";
            argument += ((this.use280 ? 1 : 0) + (this.use280 && this.PPS ? 1 : 0)).ToString() + " ";
            argument += !this.use262 ? "0" : this.com.ToString();
            return argument;
        }

        public string getBytes()
        {
            string bytes = "";
            if (this.use262)
                bytes = "160";// (bh262)";
            if (this.use262 && use280)
                bytes += " + ";
            if (this.use280)
                bytes += "160";// (bh280)";
            if (this.PPS)
                bytes += " + 768";// (bh280 PPS)";
            return bytes;
        }

        public bool isEqual(Setts other)
        {
            Type type = this.GetType();
            FieldInfo[] fields = type.GetFields();

            foreach (FieldInfo field in fields)
            {
                string fieldName = field.Name;
                //Console.WriteLine(field.GetValue(this) + " " + field.GetValue(other));
                if (field.GetValue(this).ToString() != field.GetValue(other).ToString())
                    return false;
            }

            return true;
        }

        public void copyFrom(Setts other)
        {
            Type type = this.GetType();
            FieldInfo[] fields = type.GetFields();

            foreach (FieldInfo field in fields)
            {
                string fieldName = field.Name;
                field.SetValue(this, field.GetValue(other));
            }
        }

        public void getDefault(Setts other)
        {
            Type type = this.GetType();
            FieldInfo[] fields = type.GetFields();

            foreach (FieldInfo field in fields)
            {
                string fieldName = field.Name;
                if (!fieldName.Contains("file") && !fieldName.Contains("path"))
                    field.SetValue(this, field.GetValue(other));      
            }
        }

       
    }
}
