using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace BHlauncher
{
    public partial class Form1 : Form
    {
        Setts settsC = new Setts();  // current
        Setts settsD = new Setts(); // defaults
        Setts settsF = new Setts(); // from file

        bool areLoaded = false;
        bool areDefaults = true;
        bool areUnsaved = false;

        bool itsOK = false;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            int result;
            comboBox3.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());
            //comboBox3.SelectedIndex = 0;
            //settsC.readTest();
            Console.WriteLine("enako "+settsC.isEqual(settsD));
            result = settsF.Load();

            switch (result)
            {
                case 1:     //OK, loaded
                    settsC.copyFrom(settsF);
                    areLoaded = true;
                    break;
                case -1:    //found, corrupted
                    MessageBox.Show("Config file found, but it's corrupted. Loading default settings");
                    settsC = new Setts();
                    areLoaded = false;
                    areDefaults = true;
                    break;
                case -2:    //not found
                    MessageBox.Show("Config file not found. Loading default settings");
                    settsC = new Setts();
                    areLoaded = false;
                    areDefaults = true;
                    break;
            }

           
            if (!File.Exists(settsC.RTpath+"\\"+settsC.RTfile))
            {
                MessageBox.Show("Can't find " + settsC.RTfile + ". Browse");
                OpenFileDialog findExe = new OpenFileDialog();
                findExe.InitialDirectory = Environment.CurrentDirectory;
                findExe.Title = "Find BHserver.exe";

                if (findExe.ShowDialog() == DialogResult.OK)
                {
                    settsC.RTpath = System.IO.Path.GetDirectoryName(findExe.FileName);
                    settsC.RTfile = System.IO.Path.GetFileName(findExe.FileName);
                    //settsC.path = folder;
                   
                }
                
                this.Activate();
               
            }

            if (!File.Exists(settsC.Spath + "\\" + settsC.Sfile))
            {
                MessageBox.Show("Can't find " + settsC.Sfile + ". Browse");
                OpenFileDialog findExe = new OpenFileDialog();
                findExe.InitialDirectory = Environment.CurrentDirectory;
                findExe.Title = "Find supervisory .exe";

                if (findExe.ShowDialog() == DialogResult.OK)
                {
                    settsC.Spath = System.IO.Path.GetDirectoryName(findExe.FileName);
                    settsC.Sfile = System.IO.Path.GetFileName(findExe.FileName);
                    //settsC.path = folder;

                }

                this.Activate();

            }

            fillForm();
            if (itsOK) GUIchanged();
            //itsOK = true;
            Console.WriteLine("loaded");
            timer1.Enabled = true;
        }

        public void fillForm()
        {
            itsOK = false;
            textBox2.Text = settsC.port.ToString();
            checkBox1.Checked = settsC.use280;
            
            checkBox2.Checked = settsC.use262;
            comboBox3.Enabled = settsC.use262;
            comboBox3.SelectedItem = "COM"+settsC.use262.ToString();
            checkBox3.Checked = settsC.PPS && settsC.use280;
            checkBox3.Enabled = settsC.use280;
            itsOK = true;
            //folder = sett[4];
            //file = sett[5];
            //path = folder + "\\" + file;
        }

        public void updateSetts()
        {
            settsC.port = int.Parse(textBox2.Text);
            settsC.use280 = checkBox1.Checked;
            settsC.use262 = checkBox2.Checked;// ? int.Parse(comboBox3.SelectedItem.ToString().Substring(3,1)) : 0;
            settsC.PPS = checkBox3.Checked && checkBox1.Checked;
            var selected = comboBox3.SelectedItem;
            settsC.com = selected!=null ? int.Parse(comboBox3.SelectedItem.ToString().Substring(3, 1)) : 1;
            
            //Console.WriteLine((comboBox3.SelectedItem.));

            areDefaults = settsC.isEqual(settsD);
            areUnsaved = areLoaded && settsC.isEqual(settsF) ? false : true;
        }
        void GUIchanged()
        {
            updateSetts();

            textBox4.Text = settsC.getArgument();
            
            comboBox3.Enabled = checkBox2.Checked;
            checkBox3.Enabled = checkBox1.Checked;
            checkBox3.Checked = settsC.PPS && settsC.use280;
            btnDeff.Enabled = !areDefaults;
            btnSave.Enabled = areUnsaved;
        }

        public bool IsProcessOpen(string name)
        {
            foreach (Process clsProcess in Process.GetProcesses())
            {
                if (clsProcess.ProcessName.Contains(name))
                {
                    return true;
                }
            }
            return false;
        }

        private void timer1_Tick_1(object sender, EventArgs e)
        {
            bool enable = !(IsProcessOpen(settsC.RTfile.Substring(0, (settsC.RTfile.Length) - 4))
                         || IsProcessOpen(settsC.Sfile.Substring(0, (settsC.Sfile.Length) - 4)));
            btnStart.Enabled = enable;
            btnS.Enabled = enable;

            btnStart.Text = enable ? "RealTime" : "Running..";
            btnS.Text = enable ? "SuperVisory" : "Running..";

            btnSave.Enabled = areUnsaved;
            btnDeff.Enabled = !areDefaults;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (areUnsaved == true)
            {
                var result = MessageBox.Show("Save changes?", "App closing",
                                                MessageBoxButtons.YesNoCancel,
                                                MessageBoxIcon.Question);
                if (result == DialogResult.Cancel)
                {
                    e.Cancel = true;
                }
                else
                {
                    if (result == DialogResult.Yes)
                    {
                        settsC.Save();
                    }
                }
            }
        }

        private void btnDeff_Click(object sender, EventArgs e)
        {
            //DEFAULTS
            settsC.getDefault(settsD);
            fillForm();
            if (itsOK) GUIchanged();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            //SAVE
            updateSetts();
            itsOK = false;
            settsC.Save();
            settsF.copyFrom(settsC);
            areLoaded = true;
            itsOK = true;
            if (itsOK) GUIchanged();
            //changed = false;
        }

        private void btnStartRT_Click(object sender, EventArgs e)
        {
            try
            {
                string path = settsC.RTpath + "\\" + settsC.RTfile;
                Process.Start(@path, settsC.getArgument());
                //Console.WriteLine(path + argument);
            }
            catch (Exception err)
            {
                MessageBox.Show("Sry, sth went wrong. Restart app");
            }
        }

        private void btnStartS_Click(object sender, EventArgs e)
        {
            try
            {
                string path = settsC.Spath + "\\" + settsC.Sfile;
                Process.Start(@path, settsC.getArgument());
                //Console.WriteLine(path + argument);
            }
            catch (Exception err)
            {
                MessageBox.Show("Sry, sth went wrong. Restart app");
            }
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void textBox2_TextChanged_1(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void checkBox1_CheckedChanged_1(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void checkBox2_CheckedChanged_1(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void comboBox3_SelectedValueChanged(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (itsOK) GUIchanged();
        }

        

       


        
    }
}
