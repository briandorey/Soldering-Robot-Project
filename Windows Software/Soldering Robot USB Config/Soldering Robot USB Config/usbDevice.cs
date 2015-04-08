//-----------------------------------------------------------------------------
//
//  usbDevice.cs
//
//  USB command functions for dealing with inputs and outputs on the soldering robot
//
//-----------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using usbGenericHidCommunications;
using System.Windows.Forms;
// The following namespace allows debugging output (when compiled in debug mode)
using System.Diagnostics;

namespace Soldering_Robot_USB_Config
    {
    

    /// <summary>
    /// This class contains all of the necessary functions for
    /// communicating with the soldering robot driver control board
    /// </summary>
    class usbDevice : usbGenericHidCommunication
        {

        public usbDevice(int vid, int pid) : base(vid, pid)
        {
        }


        public bool getDeviceStatus()
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x01;
            outputBuffer[2] = 0x01;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            return success;
        }

        #region Head Stepper 1 Methods

        public bool setHeadStepper1MicrosteppingMode(byte inval)  // sets the micro-stepping mode for the head 1 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x01;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getHeadStepper1MicrosteppingMode()   // gets the micro-stepping mode from the head 1 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x02;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        }

        public bool setHeadStepper1Current(byte inval)  // sets the current setting in amps for the head 1 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x03;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getHeadStepper1Current()  // gets the current setting in amps for the head 1 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x04;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        } 
        #endregion

        #region Head Stepper 2 Methods
        public bool setHeadStepper2MicrosteppingMode(byte inval)  // sets the micro-stepping mode for the head 2 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x11;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getHeadStepper2MicrosteppingMode()  // gets the micro-stepping mode from the head 2 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x12;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        }

        public bool setHeadStepper2Current(byte inval) // sets the current setting in amps for the head 2 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x13;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getHeadStepper2Current() // gets the current setting in amps for the head 2 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x14;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        } 
        #endregion

        #region Solder Stepper 1 Methods

        public bool setSolderStepper1MicrosteppingMode(byte inval)  // sets the micro-stepping mode for the solder feeder 1 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x21;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getSolderStepper1MicrosteppingMode()  // gets the micro-stepping mode from the solder feeder 1 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x22;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        }

        public bool setSolderStepper1Current(byte inval) // sets the current setting in amps for the solder feeder 1 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x23;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getSolderStepper1Current() // gets the current setting in amps for the solder feeder 1 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x24;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        } 
        #endregion

        #region Solder Stepper 2 Methods

        public bool setSolderStepper2MicrosteppingMode(byte inval)  // sets the micro-stepping mode for the solder feeder 2 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x31;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getSolderStepper2MicrosteppingMode() // gets the micro-stepping mode from the solder feeder 2 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x32;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        }

        public bool setSolderStepper2Current(byte inval) // sets the current setting in amps for the solder feeder 2 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x33;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getSolderStepper2Current() // gets the current setting in amps for the solder feeder 2 stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x34;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        }
        #endregion

        #region Caddy Stepper Methods

        public bool setCaddyStepperMicrosteppingMode(byte inval)  // sets the micro-stepping mode for the Caddy stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x41;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getCaddyStepperMicrosteppingMode()  // gets the micro-stepping mode from the Caddy stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x42;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        }

        public bool setCaddyStepperCurrent(byte inval) // sets the current setting in amps for the Caddy stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x43;
            outputBuffer[3] = inval;

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public byte getCaddyStepperCurrent() // gets the current setting in amps from the Caddy stepper driver.
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x02;
            outputBuffer[2] = 0x44;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                return inputBuffer[1];
            }
            else
            {
                return 0;
            }

        }
        #endregion

        #region Soldering Iron Control

        public bool setSolderingIron1Status(bool inval) // sets whether or not soldering iron 1 is switched on
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x01;

            if (inval)
            {
                outputBuffer[3] = 1;
            }
            else
            {
                outputBuffer[3] = 0;
            }

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public bool getSolderingIron1Status() // gets whether the soldering iron 1 element is switched on
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x02;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                if (inputBuffer[1] == 1) { return true; } else { return false; }
            }
            else
            {
                return false;
            }

        }

        public double getSolderingIron1Temperature() // gets whether the soldering iron 1 element is switched on
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];
            Byte[] doubleBuffer = new Byte[6];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x03;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                doubleBuffer[0] = inputBuffer[1];
                doubleBuffer[1] = inputBuffer[2];
                doubleBuffer[2] = inputBuffer[3];
                doubleBuffer[3] = inputBuffer[4];
                doubleBuffer[4] = inputBuffer[5];
                doubleBuffer[5] = inputBuffer[6];
                string s = Encoding.UTF8.GetString(doubleBuffer, 0, doubleBuffer.Length);
                try {
                    double f = double.Parse(s, System.Globalization.CultureInfo.InvariantCulture.NumberFormat);

                return f;
                     }
                catch { Debug.WriteLine(s); return 0; }
            }
            else
            {
                return 0;
            }

        }

        public double getSolderingIron1TargetTemperature() // gets whether the soldering iron 1 element is switched on
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];
            Byte[] doubleBuffer = new Byte[6];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x04;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                doubleBuffer[0] = inputBuffer[1];
                doubleBuffer[1] = inputBuffer[2];
                doubleBuffer[2] = inputBuffer[3];
                doubleBuffer[3] = inputBuffer[4];
                doubleBuffer[4] = inputBuffer[5];
                doubleBuffer[5] = inputBuffer[6];
                string s = Encoding.UTF8.GetString(doubleBuffer, 0, doubleBuffer.Length);
                double f = double.Parse(s, System.Globalization.CultureInfo.InvariantCulture.NumberFormat);
                return f;
            }
            else
            {
                return 0;
            }

        }

        public bool setSolderingIron1TargetTemperature(double inval) // sets whether or not soldering iron 1 is switched on
        {
            Byte[] outputBuffer = new Byte[65];

            string s = inval.ToString("000.00");
            byte[] b = Encoding.ASCII.GetBytes(s);

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x05;
            outputBuffer[3] = b[0];
            outputBuffer[4] = b[1];
            outputBuffer[5] = b[2];
            outputBuffer[6] = b[3];
            outputBuffer[7] = b[4];
            outputBuffer[8] = b[5];


            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public bool getSolderingIron1ActivationStatus() // gets whether the soldering iron 1 element is switched on
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x06;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                if (inputBuffer[1] == 1) { return true; } else { return false; }
            }
            else
            {
                return false;
            }
        }

        public bool setSolderingIron2Status(bool inval) // sets whether or not soldering iron 1 is switched on
        {
            Byte[] outputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x11;

            if (inval)
            {
                outputBuffer[3] = 1;
            }
            else { 
                outputBuffer[3] = 0; 
            }

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public bool getSolderingIron2Status() // gets whether the soldering iron 1 element is switched on
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x12;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                if (inputBuffer[1] == 1) 
                { 
                    return true; 
                } 
                else 
                { 
                    return false; 
                }
            }
            else
            {
                return false;
            }

        }

        public double getSolderingIron2Temperature() // gets whether the soldering iron 1 element is switched on
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];
            Byte[] doubleBuffer = new Byte[6];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x13;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                doubleBuffer[0] = inputBuffer[1];
                doubleBuffer[1] = inputBuffer[2];
                doubleBuffer[2] = inputBuffer[3];
                doubleBuffer[3] = inputBuffer[4];
                doubleBuffer[4] = inputBuffer[5];
                doubleBuffer[5] = inputBuffer[6];
                string s = Encoding.UTF8.GetString(doubleBuffer, 0, doubleBuffer.Length);
                try
                {
                    double f = double.Parse(s, System.Globalization.CultureInfo.InvariantCulture.NumberFormat);

                    return f;
                }
                catch { Debug.WriteLine(s); return 0; }
            }
            else
            {
                return 0;
            }

        }

        public double getSolderingIron2TargetTemperature() // gets whether the soldering iron 1 element is switched on
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];
            Byte[] doubleBuffer = new Byte[6];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x14;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                doubleBuffer[0] = inputBuffer[1];
                doubleBuffer[1] = inputBuffer[2];
                doubleBuffer[2] = inputBuffer[3];
                doubleBuffer[3] = inputBuffer[4];
                doubleBuffer[4] = inputBuffer[5];
                doubleBuffer[5] = inputBuffer[6];
                string s = Encoding.UTF8.GetString(doubleBuffer, 0, doubleBuffer.Length);
                double f = double.Parse(s, System.Globalization.CultureInfo.InvariantCulture.NumberFormat);
                return f;
            }
            else
            {
                return 0;
            }

        }

        public bool setSolderingIron2TargetTemperature(double inval) // sets whether or not soldering iron 1 is switched on
        {
            Byte[] outputBuffer = new Byte[65];

            string s = inval.ToString("000.00");
            byte[] b = Encoding.ASCII.GetBytes(s);

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x15;
            outputBuffer[3] = b[0];
            outputBuffer[4] = b[1];
            outputBuffer[5] = b[2];
            outputBuffer[6] = b[3];
            outputBuffer[7] = b[4];
            outputBuffer[8] = b[5];

            bool success;
            success = writeRawReportToDevice(outputBuffer);
            return success;
        }

        public bool getSolderingIron2ActivationStatus() // gets whether the soldering iron 1 element is switched on
        {
            Byte[] outputBuffer = new Byte[65];
            Byte[] inputBuffer = new Byte[65];

            outputBuffer[0] = 0;
            outputBuffer[1] = 0x03;
            outputBuffer[2] = 0x16;

            bool success;
            success = writeRawReportToDevice(outputBuffer);

            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
                if (inputBuffer[1] == 1) { return true; } else { return false; }
            }
            else
            {
                return false;
            }
        }

        #endregion

        // Collect debug information from the device
        public String collectDebug()
            {
            // Collect debug information from USB device
            Debug.WriteLine("Reference Application -> Collecting debug information from device");

            // Declare our output buffer
            Byte[] outputBuffer = new Byte[65];

            // Declare our input buffer
            Byte[] inputBuffer = new Byte[65];

            // Byte 0 must be set to 0
            outputBuffer[0] = 0;

            // Byte 1 must be set to our command
            outputBuffer[1] = 0x10;

            // Send the collect debug command
            writeRawReportToDevice(outputBuffer);

            // Read the response from the device
            readSingleReportFromDevice(ref inputBuffer);

            // Byte 1 contains the number of characters transfered
            if (inputBuffer[1] == 0) return String.Empty;

            // Convert the Byte array into a string of the correct length
            string s = System.Text.ASCIIEncoding.ASCII.GetString(inputBuffer, 2, inputBuffer[1]);

            return s;
            }
        }
    }