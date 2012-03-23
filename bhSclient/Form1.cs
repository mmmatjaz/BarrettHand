﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Threading;
using System.Net;

namespace bhSclient
{
    public partial class Form1 : Form
    {
        // GLOBALS
        int test = 0;
        string RdMessage;
        string IP = "127.0.0.1";
        int RXport = 4444;
        int TXport = 5555;

        // GUI thread
        UdpClient senderSock = new UdpClient();
        UdpClient receiverSock = new UdpClient();
        IPEndPoint senderEIP;
        Thread nitka;
        bool RXthreadRunning = false;
        bool shouldRun = true;
        bool isRXno = true;
        bool isTXno = true;

        public void nitkaClass()
        {
            MethodInvoker updateFormDelegate = new MethodInvoker(updateForm);
            
            receiverSock = new UdpClient(RXport);
            IPEndPoint receiverEIP = new IPEndPoint(IPAddress.Parse(IP),RXport);
            Byte[] receiveBytes;
            while (shouldRun)
            {
                try
                {

                    // Blocks until a message returns on this socket from a remote host.
                    receiveBytes = receiverSock.Receive(ref receiverEIP);
                    
                    RdMessage = Encoding.ASCII.GetString(receiveBytes);

                    Invoke(updateFormDelegate);
                    RdMessage = "";
                 }
                catch (Exception e)
                {
                    //Console.WriteLine(e.ToString());
                }
                
                //test++;
                
                
            }
            receiverSock.Close();
        }
        void updateForm()
        {
            textLogger.Text += Environment.NewLine+"REPLY: " + RdMessage;
        }
        public Form1()
        {
            InitializeComponent();
        }

        

        bool SetPar()
        {
            
            string IP = textBoxIP.Text;
            bool isRXno = int.TryParse(textBoxRX.Text, out RXport);
            bool isTXno = int.TryParse(textBoxTX.Text, out TXport);

            if (!isRXno || !isTXno)
                return true;
            else return false;
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (RXthreadRunning)
            {
                nitka.Abort();
                RXthreadRunning = false;
                textBoxIP.Enabled = true;
                textBoxRX.Enabled = true;
                textBoxTX.Enabled = true;
                buttonStart.Text = "Start";
                buttonSend.Enabled = false;
                //senderSock.Close();
            }
            else
            {
                if (isRXno && isTXno)
                {

                    SetPar();

                    nitka = new Thread(new ThreadStart(nitkaClass));
                    nitka.IsBackground = true;
                    nitka.Start();
                    RXthreadRunning = true;
                    textBoxIP.Enabled = false;
                    textBoxRX.Enabled = false;
                    textBoxTX.Enabled = false;
                    buttonStart.Text = "Stop";
                    buttonSend.Enabled = true;
                    //senderSock = new UdpClient();
                    senderEIP = new IPEndPoint(IPAddress.Parse(textBoxIP.Text), int.Parse(textBoxTX.Text));
                    //senderSock.Client.Connect(senderEIP);
                }
                else
                    MessageBox.Show("Iput port numbers");
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {       
            //senderSock.Send(System.Text.Encoding.ASCII.GetBytes(textBox262.Text), textBox262.Text.Length, senderEIP);         
            //byte[] data = Encoding.ASCII.GetBytes("This is a test message");
            string command = "@" + textBox262.Text + "@" + textBox280.Text;
            byte[] data = Encoding.ASCII.GetBytes(command);
            senderSock.Send(data, data.Length, senderEIP);
            textLogger.Text +=  Environment.NewLine+"SENT: "+ command;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            buttonSend.Enabled = false;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            receiverSock.Close();
            senderSock.Close();
        }

        private void textLogger_TextChanged(object sender, EventArgs e)
        {
            textLogger.SelectionStart = textLogger.Text.Length;
            textLogger.ScrollToCaret();
            textLogger.Refresh();
        }

        private void textBoxRX_TextChanged(object sender, EventArgs e)
        {
            isRXno = int.TryParse(textBoxRX.Text, out RXport);
        }

        private void textBoxRX_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (char.IsLetter(e.KeyChar) ||
                char.IsSymbol(e.KeyChar) ||
                char.IsWhiteSpace(e.KeyChar) ||
                char.IsPunctuation(e.KeyChar))
                e.Handled = true;
        }

        private void textBoxTX_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (char.IsLetter(e.KeyChar) ||
                char.IsSymbol(e.KeyChar) ||
                char.IsWhiteSpace(e.KeyChar) ||
                char.IsPunctuation(e.KeyChar))
                e.Handled = true;
        }

        private void textBoxTX_TextChanged(object sender, EventArgs e)
        {
            isTXno = int.TryParse(textBoxRX.Text, out TXport);
        }

        private void textBox262_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                string command = "@" + textBox262.Text + "@" + textBox280.Text;
                byte[] data = Encoding.ASCII.GetBytes(command);
                senderSock.Send(data, data.Length, senderEIP);
                textLogger.Text += Environment.NewLine + "SENT: " + command;
            }
        }

        private void textBox280_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                string command = "@" + textBox262.Text + "@" + textBox280.Text;
                byte[] data = Encoding.ASCII.GetBytes(command);
                senderSock.Send(data, data.Length, senderEIP);
                textLogger.Text += Environment.NewLine + "SENT: " + command;
            }
        }
    }
}