﻿namespace bhSclient
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabSetup = new System.Windows.Forms.TabPage();
            this.tabTerminal = new System.Windows.Forms.TabPage();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonSend = new System.Windows.Forms.Button();
            this.textBox280 = new System.Windows.Forms.TextBox();
            this.textBox262 = new System.Windows.Forms.TextBox();
            this.textLogger = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxPORT = new System.Windows.Forms.TextBox();
            this.portLabel = new System.Windows.Forms.Label();
            this.buttonStart = new System.Windows.Forms.Button();
            this.textBoxIP = new System.Windows.Forms.TextBox();
            this.tabControl1.SuspendLayout();
            this.tabSetup.SuspendLayout();
            this.tabTerminal.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabSetup);
            this.tabControl1.Controls.Add(this.tabTerminal);
            this.tabControl1.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(332, 316);
            this.tabControl1.TabIndex = 0;
            // 
            // tabSetup
            // 
            this.tabSetup.Controls.Add(this.textBoxIP);
            this.tabSetup.Controls.Add(this.portLabel);
            this.tabSetup.Controls.Add(this.textBoxPORT);
            this.tabSetup.Controls.Add(this.buttonStart);
            this.tabSetup.Controls.Add(this.label1);
            this.tabSetup.Location = new System.Drawing.Point(4, 22);
            this.tabSetup.Name = "tabSetup";
            this.tabSetup.Padding = new System.Windows.Forms.Padding(3);
            this.tabSetup.Size = new System.Drawing.Size(324, 290);
            this.tabSetup.TabIndex = 0;
            this.tabSetup.Text = "Setup";
            this.tabSetup.UseVisualStyleBackColor = true;
            // 
            // tabTerminal
            // 
            this.tabTerminal.Controls.Add(this.label5);
            this.tabTerminal.Controls.Add(this.label4);
            this.tabTerminal.Controls.Add(this.buttonSend);
            this.tabTerminal.Controls.Add(this.textBox280);
            this.tabTerminal.Controls.Add(this.textBox262);
            this.tabTerminal.Controls.Add(this.textLogger);
            this.tabTerminal.Location = new System.Drawing.Point(4, 22);
            this.tabTerminal.Name = "tabTerminal";
            this.tabTerminal.Padding = new System.Windows.Forms.Padding(3);
            this.tabTerminal.Size = new System.Drawing.Size(324, 290);
            this.tabTerminal.TabIndex = 1;
            this.tabTerminal.Text = "Terminal";
            this.tabTerminal.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(13, 10);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(25, 13);
            this.label5.TabIndex = 3;
            this.label5.Text = "262";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(121, 10);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(25, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "280";
            // 
            // buttonSend
            // 
            this.buttonSend.Location = new System.Drawing.Point(241, 5);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(75, 23);
            this.buttonSend.TabIndex = 2;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox280
            // 
            this.textBox280.Location = new System.Drawing.Point(151, 7);
            this.textBox280.Name = "textBox280";
            this.textBox280.Size = new System.Drawing.Size(70, 20);
            this.textBox280.TabIndex = 1;
            this.textBox280.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox280_KeyPress);
            // 
            // textBox262
            // 
            this.textBox262.Location = new System.Drawing.Point(39, 7);
            this.textBox262.Name = "textBox262";
            this.textBox262.Size = new System.Drawing.Size(70, 20);
            this.textBox262.TabIndex = 1;
            this.textBox262.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox262_KeyPress);
            // 
            // textLogger
            // 
            this.textLogger.Cursor = System.Windows.Forms.Cursors.IBeam;
            this.textLogger.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.textLogger.Location = new System.Drawing.Point(3, 49);
            this.textLogger.Multiline = true;
            this.textLogger.Name = "textLogger";
            this.textLogger.ReadOnly = true;
            this.textLogger.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textLogger.Size = new System.Drawing.Size(318, 238);
            this.textLogger.TabIndex = 0;
            this.textLogger.TextChanged += new System.EventHandler(this.textLogger_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(108, 105);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(17, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "IP";
            // 
            // textBoxPORT
            // 
            this.textBoxPORT.Location = new System.Drawing.Point(130, 129);
            this.textBoxPORT.Name = "textBoxPORT";
            this.textBoxPORT.Size = new System.Drawing.Size(58, 20);
            this.textBoxPORT.TabIndex = 2;
            this.textBoxPORT.Text = "4444";
            this.textBoxPORT.TextChanged += new System.EventHandler(this.textBoxTX_TextChanged);
            this.textBoxPORT.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxTX_KeyPress);
            // 
            // portLabel
            // 
            this.portLabel.AutoSize = true;
            this.portLabel.Location = new System.Drawing.Point(75, 132);
            this.portLabel.Name = "portLabel";
            this.portLabel.Size = new System.Drawing.Size(51, 13);
            this.portLabel.TabIndex = 3;
            this.portLabel.Text = "UDP port";
            // 
            // buttonStart
            // 
            this.buttonStart.Location = new System.Drawing.Point(130, 155);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(75, 23);
            this.buttonStart.TabIndex = 4;
            this.buttonStart.Text = "Start";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.button2_Click);
            // 
            // textBoxIP
            // 
            this.textBoxIP.Location = new System.Drawing.Point(130, 102);
            this.textBoxIP.Name = "textBoxIP";
            this.textBoxIP.Size = new System.Drawing.Size(93, 20);
            this.textBoxIP.TabIndex = 0;
            this.textBoxIP.Text = "127.0.0.1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(332, 316);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MinimumSize = new System.Drawing.Size(340, 346);
            this.Name = "Form1";
            this.Text = "A bit buggy Supervisory Client";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabSetup.ResumeLayout(false);
            this.tabSetup.PerformLayout();
            this.tabTerminal.ResumeLayout(false);
            this.tabTerminal.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabSetup;
        private System.Windows.Forms.TabPage tabTerminal;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.TextBox textBox280;
        private System.Windows.Forms.TextBox textBox262;
        private System.Windows.Forms.TextBox textLogger;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxIP;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.Label portLabel;
        private System.Windows.Forms.TextBox textBoxPORT;
        private System.Windows.Forms.Label label1;
    }
}

