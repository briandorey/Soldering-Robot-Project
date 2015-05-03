using System;
using System.Collections.Generic;
using System.ComponentModel;
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

namespace SolderingRobotController
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public BackgroundWorker _backgroundWorker = new BackgroundWorker();
        private usbDevice usbController;
        public AppSettings settings;
        AppConfig config = new AppConfig();
        private SolderingStationObject solderingControllerStatus = new SolderingStationObject();
        System.Windows.Threading.DispatcherTimer dispatcherTimer = new System.Windows.Threading.DispatcherTimer();

        public kflop _kflop = new kflop();
        public bool _hasHomed = false;
        private bool UsbConnected = true;
        private bool ConfigLoaded = false; 
    //    private ControllerSetup controller = new ControllerSetup();

        public kflop GetKFlop()
        {

            return _kflop;
        }
    
        

        public MainWindow()
        {
            InitializeComponent();

            usbController = new usbDevice(0x04D8, 0x0045);
            usbController.usbEvent += new usbDevice.usbEventsHandler(usbEvent_receiver);
            usbController.findTargetDevice();
            settings = new AppSettings();
            settings = config.SetDefaultConfig();

            _kflop.initdevice(usbController, settings);

            if (UsbConnected)
            {
                usbController.setSolderingIron1TargetTemperature(380.0f);
                usbController.setSolderingIron2TargetTemperature(380.0f);        
                ConfigLoaded = true;
               
                dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
               dispatcherTimer.Interval = new TimeSpan(0, 0, 2);
                dispatcherTimer.Start();
            }


            
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            // Updating the Label which displays the current second
            double target1temp = settings.SolderHead1.IronTargetTemperature;
            double actual1 = usbController.getSolderingIron1Temperature();
            if (actual1 >= target1temp)
            {
                ele1_tempreading.Foreground = new SolidColorBrush(Color.FromArgb(0xff, 0x01, 0xa4, 0x26));
            }
            else
            {
                ele1_tempreading.Foreground = new SolidColorBrush(Color.FromArgb(0xff, 0xff, 0x00, 0x00));
            }

            double target2temp = settings.SolderHead2.IronTargetTemperature;
            double actual2 = usbController.getSolderingIron2Temperature();
            if (actual2 >= target2temp)
            {
                ele2_tempreading.Foreground = new SolidColorBrush(Color.FromArgb(0xff, 0x01, 0xa4, 0x26));
            }
            else
            {
                ele2_tempreading.Foreground = new SolidColorBrush(Color.FromArgb(0xff, 0xff, 0x00, 0x00));
            }
            ele1_tempreading.Text = actual1.ToString();
            ele2_tempreading.Text = actual2.ToString();

            bool solderingiron1status = usbController.getSolderingIron1Status();
            bool solderingiron2status = usbController.getSolderingIron2Status();

            if (solderingiron1status) { lbl_iron1_status.Content = "Heating"; } else { lbl_iron1_status.Content = "Off"; }

            if (solderingiron2status) { lbl_iron2_status.Content = "Heating"; } else { lbl_iron2_status.Content = "Off"; }
         
            // Forcing the CommandManager to raise the RequerySuggested event
            CommandManager.InvalidateRequerySuggested();
        }

        void _backgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            // Do something 
        }
        private void usbEvent_receiver(object o, EventArgs e)
        {
            // Check the status of the USB device and update the form accordingly
            if (usbController.isDeviceAttached)
            {
                UsbConnected = true;
                // Device is currently attached, setup device config settings
                
            }
            else
            {
                // Device is currently unattached
                UsbConnected = false;
                // Update the status label
             //   MessageBox.Show("USB Device not connected");
            
            }
        }

        // Completed Method 
        void _backgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled)
            {
                //statusText.Text = "Cancelled";
            }
            else if (e.Error != null)
            {
                //statusText.Text = "Exception Thrown";
            }
            else
            {
                //statusText.Text = "Completed";
            }
        }

        private void ele1_home_Click(object sender, RoutedEventArgs e)
        {
            _kflop.HomeSolderHead1();
            

           
        }

        private void ele2_home_Click(object sender, RoutedEventArgs e)
        {
            _kflop.HomeSolderHead2();
            //usbController.setResetFeeder();

            //double jogspeed = 0.5;
           // _kflop.JogAxis("A", -jogspeed);
        }

        private void ele1_up_Click(object sender, RoutedEventArgs e)
        {
            _kflop.MoveSolder1Up();
            
        }

        private void ele2_up_Click(object sender, RoutedEventArgs e)
        {
            _kflop.MoveSolder2Up();
           
        }

        private void ele1_down_Click(object sender, RoutedEventArgs e)
        {
            _kflop.MoveSolder1Down();
        }

        private void ele2_down_Click(object sender, RoutedEventArgs e)
        {
            _kflop.MoveSolder2Down();
        }

        private void ele1_run_Click(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded)
            {
                dispatcherTimer.IsEnabled = false;
                if (ele1_run.IsChecked == true)
                {
                    usbController.setSolderingIron1Status(true);
                }
                else
                {
                    usbController.setSolderingIron1Status(false);
                }
                dispatcherTimer.IsEnabled = true; 
            }
        }

        private void ele2_run_Click(object sender, RoutedEventArgs e)
        {
            if (ConfigLoaded)
            {
                dispatcherTimer.IsEnabled = false;
                if (ele2_run.IsChecked == true)
                {
                    usbController.setSolderingIron2Status(true);
                }
                else
                {
                    usbController.setSolderingIron2Status(false);
                }
                dispatcherTimer.IsEnabled = true; 
            }
        }

        private void btn_Solder1Feed_Click(object sender, RoutedEventArgs e)
        {
            SetVariables();
            _kflop.FeedSolder1Go(10);
        }

       
      

        private void btn_Solder2Feed_Click(object sender, RoutedEventArgs e)
        {
            SetVariables();
            _kflop.FeedSolder2Go(10);
        }

     
       
        private void radioPinsCount1_Click(object sender, RoutedEventArgs e)
        {
            if (radioPinsCount1.IsChecked == true)
            {
               // MessageBox.Show("26");
            }
           
        }

        private void radioPinsCount2_Click(object sender, RoutedEventArgs e)
        {
            if (radioPinsCount2.IsChecked == true)
            {
              //  MessageBox.Show("40");
            }
            
        }

        private void SetVariables()
        {
           
            if (radioPinsCount1.IsChecked == true)
            {
                settings.PinsToSolder = 5;
            }
            else
            {
                settings.PinsToSolder = 20;
            }
            
            settings.PCBLocation1 = chk_pcb1.IsChecked.Value;
            settings.PCBLocation2 = chk_pcb2.IsChecked.Value;
            settings.PCBLocation3 = chk_pcb3.IsChecked.Value;
            settings.PCBLocation4 = chk_pcb4.IsChecked.Value;
            settings.PCBLocation5 = chk_pcb5.IsChecked.Value;

            settings.SolderHead1.MMPerJoint = double.Parse(txt_FeedSolder1.Text);
            settings.SolderHead2.MMPerJoint = double.Parse(txt_FeedSolder2.Text);

            settings.SolderHead1.HeatingTime = double.Parse(txt_TimeSolder1.Text);
            settings.SolderHead2.HeatingTime = double.Parse(txt_TimeSolder2.Text);
        

            _kflop.setVariables(settings);
        }
        private void btn_jobstart_Click(object sender, RoutedEventArgs e)
        {
            SetVariables();
            _kflop.startSolderJob();
        }

       

        private void btn_jobloadpcbs_Click(object sender, RoutedEventArgs e)
        {
            _kflop.LoadPCBCaddy();
        }

        private void btn_jobestop_Click(object sender, RoutedEventArgs e)
        {
            _kflop.EStop();
        }


        private void txt_FeedSolder1_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
            {
                settings.SolderHead1.MMPerJoint = double.Parse(txt_FeedSolder1.Text);
                _kflop.setVariables(settings);
            }
        }

        private void txt_FeedSolder2_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
            {
                settings.SolderHead2.MMPerJoint = double.Parse(txt_FeedSolder2.Text);
                _kflop.setVariables(settings);
            }
        }

        private void txt_TimeSolder2_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
            {
                settings.SolderHead2.HeatingTime = double.Parse(txt_TimeSolder2.Text);
                _kflop.setVariables(settings);
            }
        }

        private void txt_TimeSolder1_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
            {
                settings.SolderHead1.HeatingTime = double.Parse(txt_TimeSolder1.Text);
                _kflop.setVariables(settings);
            }
        }

      

        private void btn_TestReset_Click(object sender, RoutedEventArgs e)
        {
            _kflop.ToggleCaddyRelease();
        }

        private void btn_Solder1Load_Click(object sender, RoutedEventArgs e)
        {
            _kflop.LoadSolder(true);
        }

        private void btn_Solder2Load_Click(object sender, RoutedEventArgs e)
        {
            _kflop.LoadSolder(false);
        }

        private void chk_row1_Click(object sender, RoutedEventArgs e)
        {
            if (chk_row1.IsChecked.Value)
            {
                settings.SolderHead1.HeadRunEnable = true;
            }
            else
            {
                settings.SolderHead1.HeadRunEnable = false;
            }
            _kflop.setVariables(settings);
        }

        private void chk_row2_Click(object sender, RoutedEventArgs e)
        {
            if (chk_row2.IsChecked.Value)
            {
                settings.SolderHead2.HeadRunEnable = true;
            }
            else
            {
                settings.SolderHead2.HeadRunEnable = false;
            }
            _kflop.setVariables(settings);
        }

        private void btn_Solder1Retract_Click(object sender, RoutedEventArgs e)
        {
            _kflop.RetractSolder1();
        }

        private void btn_Solder2Reteact_Click(object sender, RoutedEventArgs e)
        {
            _kflop.RetractSolder2();
        }

      
    }
}
