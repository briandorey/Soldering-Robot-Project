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

        public usbDevice(int vid, int pid)
            : base(vid, pid)
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
            try
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
            catch
            {
                return 0;
            }

        }

        public double getSolderingIron1TargetTemperature() // gets whether the soldering iron 1 element is switched on
        {
            try
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
            catch
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
            else
            {
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
            try
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
            catch
            {
                return 0;
            }

        }

        public double getSolderingIron2TargetTemperature() // gets whether the soldering iron 1 element is switched on
        {
            try
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
            catch
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

        public SolderingStationObject getSolderingGlobalStatus() {
            SolderingStationObject tmpObj = new SolderingStationObject();

            tmpObj.success = true;

            try
            {
                Byte[] outputBuffer = new Byte[65];
                Byte[] inputBuffer = new Byte[65];

                string s = "";
                double f = 0;

                outputBuffer[0] = 0;
                outputBuffer[1] = 0x03;
                outputBuffer[2] = 0x20;

                bool success;
                success = writeRawReportToDevice(outputBuffer);

                if (success)
                {
                    // Perform the read
                    success = readSingleReportFromDevice(ref inputBuffer);


                    if (inputBuffer[3] == '1')
                    { // Iron 1 is switched on
                        tmpObj.Iron1_Running = true;
                    }
                    if (inputBuffer[3] == '0')
                    { // Iron 1 is switched on
                        tmpObj.Iron1_Running = false;
                    }
                    if (inputBuffer[5] == '1')
                    { // Iron 1 is switched on
                        tmpObj.Iron2_Running = true;
                    }
                    if (inputBuffer[5] == '0')
                    { // Iron 1 is switched on
                        tmpObj.Iron2_Running = false;
                    }

                    try
                    {

                        Byte[] SolderingIron1Temperature = new Byte[6];

                        SolderingIron1Temperature[0] = inputBuffer[7];
                        SolderingIron1Temperature[1] = inputBuffer[8];
                        SolderingIron1Temperature[2] = inputBuffer[9];
                        SolderingIron1Temperature[3] = inputBuffer[10];
                        SolderingIron1Temperature[4] = inputBuffer[11];
                        SolderingIron1Temperature[5] = inputBuffer[12];

                        s = Encoding.UTF8.GetString(SolderingIron1Temperature, 0, 6);
                        f = double.Parse(s, System.Globalization.CultureInfo.InvariantCulture.NumberFormat);

                        tmpObj.Iron1_Temperature = f;
                    }
                    catch
                    {
                        tmpObj.Iron1_Temperature = 666;
                        tmpObj.success = false;
                    }

                    try
                    {
                        Byte[] SolderingIron2Temperature = new Byte[6];

                        SolderingIron2Temperature[0] = inputBuffer[14];
                        SolderingIron2Temperature[1] = inputBuffer[15];
                        SolderingIron2Temperature[2] = inputBuffer[16];
                        SolderingIron2Temperature[3] = inputBuffer[17];
                        SolderingIron2Temperature[4] = inputBuffer[18];
                        SolderingIron2Temperature[5] = inputBuffer[19];

                        s = Encoding.UTF8.GetString(SolderingIron2Temperature, 0, 6);
                        f = double.Parse(s, System.Globalization.CultureInfo.InvariantCulture.NumberFormat);

                        tmpObj.Iron2_Temperature = f;
                    }
                    catch
                    {
                        tmpObj.Iron2_Temperature = 777;
                        tmpObj.success = false;
                    }

                    try
                    {

                        Byte[] SolderingIron1TargetTemperature = new Byte[6];

                        SolderingIron1TargetTemperature[0] = inputBuffer[21];
                        SolderingIron1TargetTemperature[1] = inputBuffer[22];
                        SolderingIron1TargetTemperature[2] = inputBuffer[23];
                        SolderingIron1TargetTemperature[3] = inputBuffer[24];
                        SolderingIron1TargetTemperature[4] = inputBuffer[25];
                        SolderingIron1TargetTemperature[5] = inputBuffer[26];

                        s = Encoding.UTF8.GetString(SolderingIron1TargetTemperature, 0, 6);
                        f = double.Parse(s, System.Globalization.CultureInfo.InvariantCulture.NumberFormat);

                        tmpObj.Iron1_Target_Temperature = f;
                    }
                    catch
                    {
                        tmpObj.Iron1_Target_Temperature = 888;
                        tmpObj.success = false;
                    }

                    try
                    {

                        Byte[] SolderingIron2TargetTemperature = new Byte[6];

                        SolderingIron2TargetTemperature[0] = inputBuffer[28];
                        SolderingIron2TargetTemperature[1] = inputBuffer[29];
                        SolderingIron2TargetTemperature[2] = inputBuffer[30];
                        SolderingIron2TargetTemperature[3] = inputBuffer[31];
                        SolderingIron2TargetTemperature[4] = inputBuffer[32];
                        SolderingIron2TargetTemperature[5] = inputBuffer[33];

                        s = Encoding.UTF8.GetString(SolderingIron2TargetTemperature, 0, 6);
                        f = double.Parse(s, System.Globalization.CultureInfo.InvariantCulture.NumberFormat);

                        tmpObj.Iron2_Target_Temperature = f;

                    }
                    catch
                    {
                        tmpObj.Iron2_Target_Temperature = 999;
                        tmpObj.success = false;
                    }

                    if (inputBuffer[35] == '1')
                    { // Iron 1 is switched on
                        tmpObj.Iron1_Activate = true;
                    }
                    if (inputBuffer[35] == '0')
                    { // Iron 1 is switched on
                        tmpObj.Iron1_Activate = false;
                    }
                    if (inputBuffer[36] == '1')
                    { // Iron 1 is switched on
                        tmpObj.Iron2_Activate = true;
                    }
                    if (inputBuffer[36] == '0')
                    { // Iron 1 is switched on
                        tmpObj.Iron2_Activate = false;
                    }

                }
                else
                {
                    tmpObj.Iron1_Activate = false;
                    tmpObj.Iron2_Activate = false;
                    tmpObj.Iron1_Running = false;
                    tmpObj.Iron2_Running = false;
                    tmpObj.Iron1_Temperature = 0;
                    tmpObj.Iron2_Temperature = 0;
                    tmpObj.Iron1_Target_Temperature = 0;
                    tmpObj.Iron2_Target_Temperature = 0;
                    tmpObj.success = false;
                }
            }
            catch
            {
                tmpObj.Iron1_Activate = false;
                tmpObj.Iron2_Activate = false;
                tmpObj.Iron1_Running = false;
                tmpObj.Iron2_Running = false;
                tmpObj.Iron1_Temperature = 0;
                tmpObj.Iron2_Temperature = 0;
                tmpObj.Iron1_Target_Temperature = 0;
                tmpObj.Iron2_Target_Temperature = 0;
                tmpObj.success = false;
            }
            

            return tmpObj;
        }

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

    public class SolderingStationObject
    {
       // Format G[Iron1 Status, 1 byte],[Iron2 Status, 1 byte],[Iron1 Temperature, 6 bytes],[Iron2 Temperature, 6 bytes],
     // [Iron1 Taget Temperature, 6 bytes],[Iron2 Target Temperature, 6 bytes], [Iron1 Activate status][Iron2 Activate status]  example G,1,1,253.22,433.22/n
        public bool Iron1_Running { get; set; }
        public bool Iron2_Running { get; set; }
        public double Iron1_Temperature { get; set; }
        public double Iron2_Temperature { get; set; }
        public double Iron1_Target_Temperature { get; set; }
        public double Iron2_Target_Temperature { get; set; }
        public bool Iron1_Activate { get; set; }
        public bool Iron2_Activate { get; set; }
        public bool success { get; set; }
    }
}

