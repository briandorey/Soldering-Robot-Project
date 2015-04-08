using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Soldering_Robot_USB_Config
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // Create an instance of the USB reference device
        private usbDevice usbController;

        private bool ConfigLoaded = false;

        public MainWindow()
        {
            InitializeComponent();

            // Create the USB reference device object (passing VID and PID)
            usbController = new usbDevice(0x04D8, 0x0045);

            // Add a listener for usb events
            usbController.usbEvent += new usbDevice.usbEventsHandler(usbEvent_receiver);

            // Perform an initial search for the target device
            usbController.findTargetDevice();
        }        

        private void usbEvent_receiver(object o, EventArgs e)
        {
            // Check the status of the USB device and update the form accordingly
            if (usbController.isDeviceAttached)
            {
                // Device is currently attached

                // Update the status label
                this.lblStatus.Content = "Connected";
                getCurrentConfig();
            }
            else
            {
                // Device is currently unattached

                // Update the status label
                //this.lblStatus.Content = "Disconnected";
            }
        }

        private void getCurrentConfig()
        {
            // get the micro-stepping modes from the controller

            byte headstepper1microsteppingmode = usbController.getHeadStepper1MicrosteppingMode();
            byte headstepper2microsteppingmode = usbController.getHeadStepper2MicrosteppingMode();
            byte solderstepper1microsteppingmode = usbController.getSolderStepper1MicrosteppingMode();
            byte solderstepper2microsteppingmode = usbController.getSolderStepper2MicrosteppingMode();
            byte caddysteppermicrosteppingmode = usbController.getCaddyStepperMicrosteppingMode();

            switch (headstepper1microsteppingmode)
            {
                case 0x01:
                    radio_HeadStepper1_Microstepping1.IsChecked = true;
                    break;
                case 0x02:
                    radio_HeadStepper1_Microstepping2.IsChecked = true;
                    break;
                case 0x04:
                    radio_HeadStepper1_Microstepping4.IsChecked = true;
                    break;
                case 0x08:
                    radio_HeadStepper1_Microstepping8.IsChecked = true;
                    break;
                case 0x16:
                    radio_HeadStepper1_Microstepping16.IsChecked = true;
                    break;
                case 0x32:
                    radio_HeadStepper1_Microstepping32.IsChecked = true;
                    break;
                default:
                    this.lblStatus.Content = "Error getting status for Head Stepper 1 Micro-stepping";
                    break;
            }

            switch (headstepper2microsteppingmode)
            {
                case 0x01:
                    radio_HeadStepper2_Microstepping1.IsChecked = true;
                    break;
                case 0x02:
                    radio_HeadStepper2_Microstepping2.IsChecked = true;
                    break;
                case 0x04:
                    radio_HeadStepper2_Microstepping4.IsChecked = true;
                    break;
                case 0x08:
                    radio_HeadStepper2_Microstepping8.IsChecked = true;
                    break;
                case 0x16:
                    radio_HeadStepper2_Microstepping16.IsChecked = true;
                    break;
                case 0x32:
                    radio_HeadStepper2_Microstepping32.IsChecked = true;
                    break;
                default:
                    this.lblStatus.Content = "Error getting status for Head Stepper 2 Micro-stepping";
                    break;
            }

            switch (solderstepper1microsteppingmode)
            {
                case 0x01:
                    radio_SolderStepper1_Microstepping1.IsChecked = true;
                    break;
                case 0x02:
                    radio_SolderStepper1_Microstepping2.IsChecked = true;
                    break;
                case 0x04:
                    radio_SolderStepper1_Microstepping4.IsChecked = true;
                    break;
                case 0x08:
                    radio_SolderStepper1_Microstepping8.IsChecked = true;
                    break;
                case 0x16:
                    radio_SolderStepper1_Microstepping16.IsChecked = true;
                    break;
                case 0x32:
                    radio_SolderStepper1_Microstepping32.IsChecked = true;
                    break;
                default:
                    this.lblStatus.Content = "Error getting status for Solder Stepper 1 Micro-stepping";
                    break;
            }

            switch (solderstepper2microsteppingmode)
            {
                case 0x01:
                    radio_SolderStepper2_Microstepping1.IsChecked = true;
                    break;
                case 0x02:
                    radio_SolderStepper2_Microstepping2.IsChecked = true;
                    break;
                case 0x04:
                    radio_SolderStepper2_Microstepping4.IsChecked = true;
                    break;
                case 0x08:
                    radio_SolderStepper2_Microstepping8.IsChecked = true;
                    break;
                case 0x16:
                    radio_SolderStepper2_Microstepping16.IsChecked = true;
                    break;
                case 0x32:
                    radio_SolderStepper2_Microstepping32.IsChecked = true;
                    break;
                default:
                    this.lblStatus.Content = "Error getting status for Solder Stepper 2 Micro-stepping";
                    break;
            }

            switch (caddysteppermicrosteppingmode)
            {
                case 0x01:
                    radio_CaddyStepper_Microstepping1.IsChecked = true;
                    break;
                case 0x02:
                    radio_CaddyStepper_Microstepping2.IsChecked = true;
                    break;
                case 0x04:
                    radio_CaddyStepper_Microstepping4.IsChecked = true;
                    break;
                case 0x08:
                    radio_CaddyStepper_Microstepping8.IsChecked = true;
                    break;
                case 0x16:
                    radio_CaddyStepper_Microstepping16.IsChecked = true;
                    break;
                case 0x32:
                    radio_CaddyStepper_Microstepping32.IsChecked = true;
                    break;
                default:
                    this.lblStatus.Content = "Error getting status for Caddy Stepper Micro-stepping";
                    break;
            }

            // Get the default current settings from the controller

            byte headstepper1current = usbController.getHeadStepper1Current();
            byte headstepper2current = usbController.getHeadStepper2Current();
            byte solderstepper1current = usbController.getSolderStepper1Current();
            byte solderstepper2current = usbController.getSolderStepper2Current();
            byte caddysteppercurrent = usbController.getCaddyStepperCurrent();

            slider_HeadStepper1_Current.Value = headstepper1current;
            lbl_HeadStepper1_Current.Content = calculateCurrent_mA(headstepper1current).ToString("0.00 A");

            slider_HeadStepper2_Current.Value = headstepper2current;
            lbl_HeadStepper2_Current.Content = calculateCurrent_mA(headstepper2current).ToString("0.00 A");

            slider_SolderStepper1_Current.Value = solderstepper1current;
            lbl_SolderStepper1_Current.Content = calculateCurrent_mA(solderstepper1current).ToString("0.00 A");

            slider_SolderStepper2_Current.Value = solderstepper2current;
            lbl_SolderStepper2_Current.Content = calculateCurrent_mA(solderstepper2current).ToString("0.00 A");

            slider_CaddyStepper_Current.Value = caddysteppercurrent;
            lbl_CaddyStepper_Current.Content = calculateCurrent_mA(caddysteppercurrent).ToString("0.00 A");

            // get the soldering iron status

            double solderingiron1temperature = usbController.getSolderingIron1Temperature();
            lbl_iron1_currenttemperature.Content = solderingiron1temperature.ToString("0.00°C");
            
            double solderingiron2temperature = usbController.getSolderingIron2Temperature();
            lbl_iron2_currenttemperature.Content = solderingiron2temperature.ToString("0.00°C");
            
            double solderingiron1targettemperature = usbController.getSolderingIron1TargetTemperature();
            txt_iron1_targettemperature.Text = solderingiron1targettemperature.ToString("0.00");

            Double solderingiron2targettemperature = usbController.getSolderingIron2TargetTemperature();                  
            txt_iron2_targettemperature.Text = solderingiron2targettemperature.ToString("0.00");
            
            bool solderingiron1status = usbController.getSolderingIron1Status();
            bool solderingiron2status = usbController.getSolderingIron2Status();            

            if (solderingiron1status) { lbl_iron1_status.Content = "Heating"; } else { lbl_iron1_status.Content = "Off"; }

            if (solderingiron2status) { lbl_iron2_status.Content = "Heating"; } else { lbl_iron2_status.Content = "Off"; }

            bool solderingiron1activation = usbController.getSolderingIron1ActivationStatus();
            bool solderingiron2activation = usbController.getSolderingIron2ActivationStatus();

            if (solderingiron1activation) { cb_solderingiron1active.IsChecked = true; } else { cb_solderingiron1active.IsChecked = false; }
            if (solderingiron2activation) { cb_solderingiron2active.IsChecked = true; } else { cb_solderingiron2active.IsChecked = false; }
            
            ConfigLoaded = true;

            // start the timer to update the temperatures
            System.Windows.Threading.DispatcherTimer dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += dispatcherTimer_Tick;
            dispatcherTimer.Interval = new TimeSpan(0,0,1);
            dispatcherTimer.Start();
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            // get the temperatures from the soldering irons and display it on the screen

            double solderingiron1temperature = usbController.getSolderingIron1Temperature();
            lbl_iron1_currenttemperature.Content = solderingiron1temperature.ToString("0.00°C");

            double solderingiron2temperature = usbController.getSolderingIron2Temperature();
            lbl_iron2_currenttemperature.Content = solderingiron2temperature.ToString("0.00°C");

            bool solderingiron1status = usbController.getSolderingIron1Status();
            bool solderingiron2status = usbController.getSolderingIron2Status();

            if (solderingiron1status) { lbl_iron1_status.Content = "Heating"; } else { lbl_iron1_status.Content = "Off"; }

            if (solderingiron2status) { lbl_iron2_status.Content = "Heating"; } else { lbl_iron2_status.Content = "Off"; }
        }

        private void btConnect_Click(object sender, RoutedEventArgs e)
        {
            // search for the USB device and connect if available

            usbController.findTargetDevice();
            if (usbController.getDeviceStatus())
            {
                this.lblStatus.Content = "Connected";
            }
            else
            {
                this.lblStatus.Content = "Connection Failed";
            }
        }

        // set the micro-stepping modes and current levels

        private void radio_HeadStepper1_Microstepping1_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) {  usbController.setHeadStepper1MicrosteppingMode(0x01);  }
        }

        private void radio_HeadStepper1_Microstepping2_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) {  usbController.setHeadStepper1MicrosteppingMode(0x02);  }
        }

        private void radio_HeadStepper1_Microstepping4_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) {  usbController.setHeadStepper1MicrosteppingMode(0x04);  }
        }

        private void radio_HeadStepper1_Microstepping8_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded)  {  usbController.setHeadStepper1MicrosteppingMode(0x08); }
        }

        private void radio_HeadStepper1_Microstepping16_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded)  {  usbController.setHeadStepper1MicrosteppingMode(0x16); }
        }

        private void radio_HeadStepper1_Microstepping32_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded)  {  usbController.setHeadStepper1MicrosteppingMode(0x32); }
        }

        private void slider_HeadStepper1_Current_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (ConfigLoaded)
            {
                byte newvalue = System.Convert.ToByte(slider_HeadStepper1_Current.Value);
                usbController.setHeadStepper1Current(newvalue);

                lbl_HeadStepper1_Current.Content = calculateCurrent_mA(newvalue).ToString("0.00 A");
            }
        }
       

        private void radio_HeadStepper2_Microstepping1_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setHeadStepper2MicrosteppingMode(0x01); }
        }

        private void radio_HeadStepper2_Microstepping2_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setHeadStepper2MicrosteppingMode(0x02); }
        }

        private void radio_HeadStepper2_Microstepping4_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setHeadStepper2MicrosteppingMode(0x04); }
        }

        private void radio_HeadStepper2_Microstepping8_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setHeadStepper2MicrosteppingMode(0x08); }
        }

        private void radio_HeadStepper2_Microstepping16_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setHeadStepper2MicrosteppingMode(0x16); }
        }

        private void radio_HeadStepper2_Microstepping32_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setHeadStepper2MicrosteppingMode(0x32); }
        }

        private void slider_HeadStepper2_Current_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (ConfigLoaded)
            {
                byte newvalue = System.Convert.ToByte(slider_HeadStepper2_Current.Value);
                usbController.setHeadStepper2Current(newvalue);

                lbl_HeadStepper2_Current.Content = calculateCurrent_mA(newvalue).ToString("0.00 A");
            }
        }

        private void radio_SolderStepper1_Microstepping1_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper1MicrosteppingMode(0x01); }
        }

        private void radio_SolderStepper1_Microstepping2_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper1MicrosteppingMode(0x02); }
        }

        private void radio_SolderStepper1_Microstepping4_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper1MicrosteppingMode(0x04); }
        }

        private void radio_SolderStepper1_Microstepping8_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper1MicrosteppingMode(0x08); }
        }

        private void radio_SolderStepper1_Microstepping16_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper1MicrosteppingMode(0x16); }
        }

        private void radio_SolderStepper1_Microstepping32_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper1MicrosteppingMode(0x32); }
        }

        private void slider_SolderStepper1_Current_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (ConfigLoaded)
            {
                byte newvalue = System.Convert.ToByte(slider_SolderStepper1_Current.Value);
                usbController.setSolderStepper1Current(newvalue);

                lbl_SolderStepper1_Current.Content = calculateCurrent_mA(newvalue).ToString("0.00 A");
            }
        }

        private void radio_SolderStepper2_Microstepping1_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper2MicrosteppingMode(0x01); }
        }

        private void radio_SolderStepper2_Microstepping2_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper2MicrosteppingMode(0x02); }
        }

        private void radio_SolderStepper2_Microstepping4_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper2MicrosteppingMode(0x04); }
        }

        private void radio_SolderStepper2_Microstepping8_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper2MicrosteppingMode(0x08); }
        }

        private void radio_SolderStepper2_Microstepping16_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper2MicrosteppingMode(0x16); }
        }

        private void radio_SolderStepper2_Microstepping32_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setSolderStepper2MicrosteppingMode(0x32); }
        }

        private void slider_SolderStepper2_Current_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (ConfigLoaded)
            {
                byte newvalue = System.Convert.ToByte(slider_SolderStepper2_Current.Value);
                usbController.setSolderStepper2Current(newvalue);

                lbl_SolderStepper2_Current.Content = calculateCurrent_mA(newvalue).ToString("0.00 A");
            }
        }

        private void radio_CaddyStepper_Microstepping1_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setCaddyStepperMicrosteppingMode(0x01); }
        }

        private void radio_CaddyStepper_Microstepping2_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setCaddyStepperMicrosteppingMode(0x02); }
        }

        private void radio_CaddyStepper_Microstepping4_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setCaddyStepperMicrosteppingMode(0x04); }
        }

        private void radio_CaddyStepper_Microstepping8_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setCaddyStepperMicrosteppingMode(0x08); }
        }

        private void radio_CaddyStepper_Microstepping16_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setCaddyStepperMicrosteppingMode(0x16); }
        }

        private void radio_CaddyStepper_Microstepping32_Checked(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded) { usbController.setCaddyStepperMicrosteppingMode(0x32); }
        }

        private void slider_CaddyStepper_Current_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (ConfigLoaded)
            {
                byte newvalue = System.Convert.ToByte(slider_CaddyStepper_Current.Value);
                usbController.setCaddyStepperCurrent(newvalue);

                lbl_CaddyStepper_Current.Content = calculateCurrent_mA(newvalue).ToString("0.00 A");
            }
        }

        // calculate the current in milliamps based on the shunt resistor value and reference voltage

        private double calculateCurrent_mA(byte inval)
        {
            double setvoltage = System.Convert.ToDouble(inval);
            double refvoltage = setvoltage * 0.012890625;
            double resistance = 3.1;
            double current = refvoltage / resistance;

            return current;
        }

        // send the new soldering iron target temperature to the controller

        private void bt_iron1_save_Click(object sender, RoutedEventArgs e)
        {
            usbController.setSolderingIron1TargetTemperature(double.Parse(txt_iron1_targettemperature.Text));
        }

        private void bt_iron2_save_Click(object sender, RoutedEventArgs e)
        {
            usbController.setSolderingIron2TargetTemperature(double.Parse(txt_iron2_targettemperature.Text));
        }

        // set the active status for the soldering irons

        private void cb_solderingiron1active_Checked(object sender, RoutedEventArgs e)
        {
            if (cb_solderingiron1active.IsChecked == true) { 
                usbController.setSolderingIron1Status(true); 
            }
            else
            {
                usbController.setSolderingIron1Status(false); 
            }
        }

        private void cb_solderingiron2active_Checked(object sender, RoutedEventArgs e)
        {
            if (cb_solderingiron2active.IsChecked == true)
            {
                usbController.setSolderingIron2Status(true);
            }
            else
            {
                usbController.setSolderingIron2Status(false);
            }
        }
    }
}
