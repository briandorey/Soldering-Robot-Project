using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using KMotion_dotNet;
using System.Reflection;
using System.IO;
using System.Threading;
using System.Diagnostics;
using System.Windows;
using System.ComponentModel;

namespace SolderingRobotController
{
    public class kflop
    {
        public KM_Controller _Controller;
        AppSettings settings;
        KM_Axis _Solder1Move;
        KM_Axis _Solder2Move;
        KM_Axis _Solder1Feed;
        KM_Axis _Solder2Feed;
        KM_Axis _CaddyDrive;
        KM_Axis _CAxis;
        KM_CoordMotion _Motion;
        bool caddyEnabled = true;
    
        private readonly BackgroundWorker backgroundWorkerSolderJob = new BackgroundWorker();
        usbDevice usb;

        App MyApplication;

        private double currentSolder1Move = 0.0;
        private double currentSolder2Move = 0.0;
        private double currentSolder1Feed = 0.0;
        private double currentSolder2Feed = 0.0;
        private double currentCaddy = 0.0;

        private double cpmCaddy = 160;

        public double[] dblCleanerLocations = { 75.0,165.0,255.0,345.0,435.0};
        public double[] dblPCBLocations = { 5.6,95.4,185.4,275.4,365.4};

        public bool eStopActive = false;     

        
        public void initdevicesettings()
        {
           
            MyApplication = ((App)Application.Current);
           

            string codeBase = Assembly.GetExecutingAssembly().CodeBase;
            UriBuilder uri = new UriBuilder(codeBase);
            string path = Uri.UnescapeDataString(uri.Path);
            path = Path.GetDirectoryName(path);
            path = Path.GetDirectoryName(path);
            path = Path.GetDirectoryName(path);
            
            String TheCFile = "C:\\KMotion432\\KMotion\\Release\\InitSolderingRobot.c";
            String result = _Controller.ExecuteProgram(1, TheCFile, false);
            if (result != "") MessageBox.Show(result);
            // setup and map kflop axis to variables
            _Solder1Move = new KMotion_dotNet.KM_Axis(_Controller, 0, "x");
            _Solder2Move = new KMotion_dotNet.KM_Axis(_Controller, 1, "y");
            _Solder1Feed = new KMotion_dotNet.KM_Axis(_Controller, 2, "z");
            _Solder2Feed = new KMotion_dotNet.KM_Axis(_Controller, 3, "a");
            _CaddyDrive = new KMotion_dotNet.KM_Axis(_Controller, 4, "b");
            _CAxis = new KMotion_dotNet.KM_Axis(_Controller, 5, "c");

            _Motion = new KMotion_dotNet.KM_CoordMotion(_Controller);


            Debug.WriteLine(_Controller.GetCommandValue<string>("Version", false));

            _Solder1Move.Enable();
            _Solder2Move.Enable();
            _Solder1Feed.Enable();
            _Solder2Feed.Enable();
            _CaddyDrive.Enable();
          
            _Controller.WriteLine("SetBitDirection38=0");
            caddyEnabled = true;
            // init IO ports with default values

            _Solder1Move.CPU = settings.SolderHead1.HeadCPM;
            _Solder1Move.Velocity = settings.SolderHead1.HeadVelocity;
            _Solder1Move.Acceleration = settings.SolderHead1.HeadAcceleration;
            _Solder1Move.TuningParams.Jerk = settings.SolderHead1.HeadJerk;


            _Solder2Move.CPU = settings.SolderHead2.HeadCPM;
            _Solder2Move.Velocity = settings.SolderHead2.HeadVelocity;
            _Solder2Move.Acceleration = settings.SolderHead2.HeadAcceleration;
            _Solder2Move.TuningParams.Jerk = settings.SolderHead2.HeadJerk;

            _Solder1Feed.CPU = settings.SolderHead1.FeedCPM;
            _Solder1Feed.Velocity = settings.SolderHead1.FeedVelocity;
            _Solder1Feed.Acceleration = settings.SolderHead1.FeedAcceleration;
            _Solder1Feed.TuningParams.Jerk = settings.SolderHead1.FeedJerk;

            _Solder2Feed.CPU = settings.SolderHead2.FeedCPM;
            _Solder2Feed.Velocity = settings.SolderHead2.FeedVelocity;
            _Solder2Feed.Acceleration = settings.SolderHead2.FeedAcceleration;
            _Solder2Feed.TuningParams.Jerk = settings.SolderHead2.FeedJerk;

            _CaddyDrive.CPU = cpmCaddy;
            _CaddyDrive.Velocity = 2e4;
            _CaddyDrive.Acceleration = 2e4;
            _CaddyDrive.TuningParams.Jerk = 3e5;
            _CaddyDrive.TuningParams.OutputGain = -1;

            // setup homing params
            _Solder1Move.HomingParams.SourceType = HOMING_ROUTINE_SOURCE_TYPE.AUTO;
            _Solder1Move.HomingParams.DefaultThread = 5;
            _Solder1Move.HomingParams.HomeFastVel = 600;
            _Solder1Move.HomingParams.HomeSlowVel = 200;
            _Solder1Move.HomingParams.HomeLimitBit = 39;
            _Solder1Move.HomingParams.HomeLimitState = true;
            _Solder1Move.HomingParams.RepeatHomeAtSlowerRate = true;
            _Solder1Move.HomingParams.SequencePriority = 1;
            _Solder1Move.HomingParams.HomeNegative = true;
            _Solder1Move.HomingParams.StatusBit = 39;
            _Solder1Move.HomingParams.SetToZero = true;


            _Solder2Move.HomingParams.SourceType = HOMING_ROUTINE_SOURCE_TYPE.AUTO;
            _Solder2Move.HomingParams.DefaultThread = 3;
            _Solder2Move.HomingParams.HomeFastVel = 600;
            _Solder2Move.HomingParams.HomeSlowVel = 200;
            _Solder2Move.HomingParams.HomeLimitBit = 40;
            _Solder2Move.HomingParams.HomeLimitState = true;
            _Solder2Move.HomingParams.RepeatHomeAtSlowerRate = true;
            _Solder2Move.HomingParams.SequencePriority = 2;
            _Solder2Move.HomingParams.HomeNegative = true;
            _Solder2Move.HomingParams.StatusBit = 40;
            _Solder2Move.HomingParams.SetToZero = true;


            _CaddyDrive.HomingParams.SourceType = HOMING_ROUTINE_SOURCE_TYPE.AUTO;
            _CaddyDrive.HomingParams.DefaultThread = 4;
            _CaddyDrive.HomingParams.HomeFastVel = 2000;
            _CaddyDrive.HomingParams.HomeSlowVel = 100;
            _CaddyDrive.HomingParams.HomeLimitBit = 43;
            _CaddyDrive.HomingParams.HomeLimitState = true;
            _CaddyDrive.HomingParams.RepeatHomeAtSlowerRate = true;
            _CaddyDrive.HomingParams.SequencePriority = 4;
            _CaddyDrive.HomingParams.HomeNegative = true;
            _CaddyDrive.HomingParams.StatusBit = 43;
            _CaddyDrive.HomingParams.SetToZero = true;

        }

        public void initdevice(usbDevice _usb, AppSettings _settings)
        {
            settings = _settings;
            usb = _usb;
            Debug.WriteLine("Init Device");
            _Controller = new KMotion_dotNet.KM_Controller();
          //  AddHandlers();

            
               if (_Controller.BoardID > 0)
               {
                 
                  initdevicesettings();
                  backgroundWorkerSolderJob.DoWork += worker_DoWork;
                  backgroundWorkerSolderJob.RunWorkerCompleted += worker_RunWorkerCompleted;
                  backgroundWorkerSolderJob.WorkerSupportsCancellation = true;

                 

                 
               }
               else
               {
                   MessageBox.Show("Kflop not detected");
               }
        }


        // public methods
        public void LoadPCBCaddy()
        {
            // launch loading in a new thread
           // _CaddyDrive.Disable();
            Thread runLoadingCaddy = new Thread(new ThreadStart(RunPCBCaddy));
            runLoadingCaddy.Start();
        }
        private void RunPCBCaddy()
        {
            setAlltoZero();
          //  _EnableCaddy.SetDigitalValue(false);
            _Controller.WriteLine("SetBitDirection38=0");
            caddyEnabled = true;
            _Solder1Move.StartDoHome();
            _Solder2Move.StartDoHome();
            while (!_Solder1Move.MotionComplete())
            {
                Thread.Sleep(50);
            }

            
            while (!_Solder2Move.MotionComplete())
            {
                Thread.Sleep(50);
            }
          
            _CaddyDrive.StartDoHome();
            while (!_CaddyDrive.MotionComplete())
            {
                Thread.Sleep(50);
            }
          
            
         //   _EnableCaddy.SetDigitalValue(true);
        }
        public void ToggleCaddyRelease()
        {
            if (caddyEnabled)
            {
                _Controller.WriteLine("SetBitDirection38=1");
                caddyEnabled = false;

            }
            else
            {
                _Controller.WriteLine("SetBitDirection38=0");
                caddyEnabled = true;
            }

        }
        public void HomeSolderHead1()
        {
            // launch loading in a new thread
            Thread runHomeSolderHead1 = new Thread(new ThreadStart(RunHomeSolderHead1));
            runHomeSolderHead1.Start();
        }
        private void RunHomeSolderHead1()
        {
            setAlltoZero();
            _Solder1Move.StartDoHome();
            while (!_Solder1Move.MotionComplete())
            {
                Thread.Sleep(50);
            }
            setAlltoZero();
        }
        public void HomeSolderHead2()
        {
            // launch loading in a new thread
            Thread runHomeSolderHead2 = new Thread(new ThreadStart(RunHomeSolderHead2));
            runHomeSolderHead2.Start();
        }
        private void RunHomeSolderHead2()
        {
            setAlltoZero();
            _Solder2Move.StartDoHome();
            while (!_Solder2Move.MotionComplete())
            {
                Thread.Sleep(50);
            }
           
        }

        public void MoveSolder1Up()
        {
            currentSolder1Move = 0;
            DoMove();
           
        }
       
        public void MoveSolder1Down()
        {
            currentSolder1Move = settings.SolderHead1.PCBHeight;
            DoMove();
        } 
        
        public void MoveSolder2Up()
        {
            currentSolder2Move = 0;
            DoMove();

        }
        public void MoveSolder2Down()
        {
            currentSolder2Move = settings.SolderHead2.PCBHeight;
            DoMove();
        }
        public void FeedSolder1Go(double distance)
        {
          
            _Solder1Feed.RelativeMoveTo(distance);
       
            
          
        }
        public void FeedSolder2Go(double distance)
        {
            _Solder2Feed.RelativeMoveTo(distance);
         
        }
        public void LoadSolder(bool val)
        {
          
            if (val)
            {
                _Solder1Feed.Velocity = settings.SolderHead1.LoadingFeedVelocity;
                _Solder1Feed.Acceleration = settings.SolderHead1.LoadingFeedAcceleration;
                _Solder1Feed.TuningParams.Jerk = settings.SolderHead1.LoadingFeedJerk;

                _Solder1Feed.RelativeMoveTo(settings.SolderHead1.LoadSolderDistance);

                _Solder1Feed.Velocity = settings.SolderHead1.FeedVelocity;
                _Solder1Feed.Acceleration = settings.SolderHead1.FeedAcceleration;
                _Solder1Feed.TuningParams.Jerk = settings.SolderHead1.FeedJerk;
            }
            else
            {
                _Solder2Feed.Velocity = settings.SolderHead2.LoadingFeedVelocity;
                _Solder2Feed.Acceleration = settings.SolderHead2.LoadingFeedAcceleration;
                _Solder2Feed.TuningParams.Jerk = settings.SolderHead2.LoadingFeedJerk;

                _Solder2Feed.RelativeMoveTo(settings.SolderHead1.LoadSolderDistance);

                _Solder2Feed.Velocity = settings.SolderHead2.FeedVelocity;
                _Solder2Feed.Acceleration = settings.SolderHead2.FeedAcceleration;
                _Solder2Feed.TuningParams.Jerk = settings.SolderHead2.FeedJerk;
            }

        }
        private void RunSolderSlow()
        {
            _Solder1Feed.Velocity = settings.SolderHead1.FeedVelocity;
            _Solder1Feed.Acceleration = settings.SolderHead1.FeedAcceleration;
            _Solder1Feed.TuningParams.Jerk = settings.SolderHead1.FeedJerk;
            _Solder2Feed.Velocity = settings.SolderHead2.FeedVelocity;
            _Solder2Feed.Acceleration = settings.SolderHead2.FeedAcceleration;
            _Solder2Feed.TuningParams.Jerk = settings.SolderHead2.FeedJerk;
        }
        private void RunSolderFast()
        {
            _Solder1Feed.Velocity = settings.SolderHead1.LoadingFeedVelocity;
            _Solder1Feed.Acceleration = settings.SolderHead1.LoadingFeedAcceleration;
            _Solder1Feed.TuningParams.Jerk = settings.SolderHead1.LoadingFeedJerk;
            _Solder2Feed.Velocity = settings.SolderHead2.LoadingFeedVelocity;
            _Solder2Feed.Acceleration = settings.SolderHead2.LoadingFeedAcceleration;
            _Solder2Feed.TuningParams.Jerk = settings.SolderHead2.LoadingFeedJerk;
        }

        public void setAlltoZero()
        {
            Debug.WriteLine("Setting All to Zero");
            currentSolder1Move = 0.0;
            currentSolder2Move = 0.0;
            currentCaddy = 0.0;
            _Solder1Move.SetCurrentPosition(0);
            _Solder2Move.SetCurrentPosition(0);
            _CaddyDrive.SetCurrentPosition(0);
       
        }
       

        public bool DoMove()
        {
            _CaddyDrive.StartMoveTo(currentCaddy);
            _Solder1Move.StartMoveTo(currentSolder1Move);
            _Solder2Move.StartMoveTo(currentSolder2Move);
          

            while (!_Solder1Move.MotionComplete())
            {
                Thread.Sleep(10);
            }
            while (!_Solder2Move.MotionComplete())
            {
                Thread.Sleep(10);
            }
            while (!_CaddyDrive.MotionComplete())
            {
                Thread.Sleep(10);
            }
          
            Debug.WriteLine("(currentSolder1Move:" + currentSolder1Move.ToString() + ")(currentSolder2Move:" + currentSolder2Move.ToString() + ")(currentSolder1Feed:" + currentSolder1Feed.ToString() + ")(currentSolder2Feed:" + currentSolder2Feed.ToString() + ")(currentCaddy:" + currentCaddy.ToString());
            return true;
        }

        public void EStop()
        {
            if (backgroundWorkerSolderJob.IsBusy)
            {
                backgroundWorkerSolderJob.CancelAsync();
            }
            if (!eStopActive)
            {
                eStopActive = true;
                _Solder1Move.Disable();
                _Solder2Move.Disable();
                _Solder1Feed.Disable();
                _Solder2Feed.Disable();
                _CaddyDrive.Disable();
             
               
            }
            else
            {
                _Solder1Move.Enable();
                _Solder2Move.Enable();
                _Solder1Feed.Enable();
                _Solder2Feed.Enable();
                _CaddyDrive.Enable();
         
                eStopActive = false;
              
            }
            setAlltoZero();
        }

      
        // setup local variables
        public void setVariables(AppSettings _settings)
        {
            settings = _settings;

        }

        /// pcb soldering section
        /// 
       

        public void startSolderJob()
        {
            if (!this.backgroundWorkerSolderJob.IsBusy)
            {
                this.backgroundWorkerSolderJob.RunWorkerAsync();
            }
        }


        private void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            _Controller.WriteLine("SetBitDirection38=0");
            caddyEnabled = true;
            BackgroundWorker worker = sender as BackgroundWorker;
            bool hasCancelled = false;
            int totalrows = settings.PinsToSolder;
         
            setAlltoZero();
            double intheatingDelay = settings.SolderHead1.HeatingTime / 10;
            double intheatingDelay2 = settings.SolderHead2.HeatingTime / 10;
            
            // check if pcb is in location 1
            if (settings.PCBLocation1) {
                // build pcb in location 1
               
                while (!DoBoardSolderRun(totalrows, intheatingDelay, intheatingDelay2, 0))
                {
                    Thread.Sleep(10);

                    if (backgroundWorkerSolderJob.CancellationPending)
                    {
                        e.Cancel = true;
                        hasCancelled = true;
                        break;
                    }
                }               
            }


            if (settings.PCBLocation2 && !hasCancelled)
            {
                // build pcb in location 1
               
                while (!DoBoardSolderRun(totalrows, intheatingDelay, intheatingDelay2, 1))
                {
                    Thread.Sleep(10);

                    if (backgroundWorkerSolderJob.CancellationPending)
                    {
                        e.Cancel = true;
                        hasCancelled = true;
                        break;
                    }
                }
            }

            if (settings.PCBLocation3 && !hasCancelled)
            {
                // build pcb in location 1
                
                while (!DoBoardSolderRun(totalrows, intheatingDelay, intheatingDelay2, 2))
                {
                    Thread.Sleep(10);

                    if (backgroundWorkerSolderJob.CancellationPending)
                    {
                        e.Cancel = true;
                        break;
                    }
                }
            }
            if (settings.PCBLocation4 && !hasCancelled)
            {
                // build pcb in location 1
               
                while (!DoBoardSolderRun(totalrows, intheatingDelay, intheatingDelay2, 3))
                {
                    Thread.Sleep(10);

                    if (backgroundWorkerSolderJob.CancellationPending)
                    {
                        e.Cancel = true;
                        hasCancelled = true;
                        break;
                    }
                }
            }
            if (settings.PCBLocation5 && !hasCancelled)
            {
                // build pcb in location 1
               
                while (!DoBoardSolderRun(totalrows, intheatingDelay, intheatingDelay2, 4))
                {
                    Thread.Sleep(10);

                    if (backgroundWorkerSolderJob.CancellationPending)
                    {
                        e.Cancel = true;
                        hasCancelled = true;
                        break;
                    }
                }
            }
     

           // _EnableCaddy.SetDigitalValue(true);
            backgroundWorkerSolderJob.CancelAsync();
            _Solder1Move.StartDoHome();
            _Solder2Move.StartDoHome();
            while (!_Solder1Move.MotionComplete())
            {
                Thread.Sleep(50);
            }


            while (!_Solder2Move.MotionComplete())
            {
                Thread.Sleep(50);
            }

           // _CaddyDrive.MoveTo(500);
          //  while (!_CaddyDrive.MotionComplete())
          //  {
          //      Thread.Sleep(50);
          //  }
           
            _Controller.WriteLine("SetBitDirection38=1");
            caddyEnabled = false;
        }
        private bool DoBoardSolderRun(int totalrows, double intheatingDelay, double intheatingDelay2, int CleaningLoc)
        {
            int currentrow = 0;
            bool hasfinsihed = false;

            if (settings.SolderHead1.HeadRunEnable)
            {
                currentSolder1Move = settings.SolderHead1.RetractDistance;
            }
            if (settings.SolderHead2.HeadRunEnable)
            {
                currentSolder2Move = settings.SolderHead2.RetractDistance;
            }

           
            
            
            hasfinsihed = DoMove();
            while (!hasfinsihed)
            {
                Thread.Sleep(10);
            }

            currentCaddy = dblPCBLocations[CleaningLoc];
            hasfinsihed = DoMove();
            while (!hasfinsihed)
            {
                Thread.Sleep(10);
            }
            Thread.Sleep(500);

            while (currentrow < totalrows)
            {
                if (settings.SolderHead1.HeadRunEnable)
                {
                    currentSolder1Move = settings.SolderHead1.PCBHeight;
                }
                if (settings.SolderHead2.HeadRunEnable)
                {
                    currentSolder2Move = settings.SolderHead2.PCBHeight; 
                }
                
                
                hasfinsihed = DoMove();

               
                while (!hasfinsihed)
                {
                    Thread.Sleep(10);
                }
                if (backgroundWorkerSolderJob.CancellationPending)
                {
                    break;
                }

                int counter = 1;
                while (counter <= intheatingDelay)
                {
                    Thread.Sleep(10);
                    counter++;
                }
                if (backgroundWorkerSolderJob.CancellationPending)
                {
                    break;
                }
                if (settings.SolderHead1.HeadRunEnable)
                {
                    _Solder1Feed.StartRelativeMoveTo(settings.SolderHead1.MMPerJoint + settings.SolderHead1.SolderRetractDistance);
                }
                if (settings.SolderHead2.HeadRunEnable)
                {
                    _Solder2Feed.StartRelativeMoveTo(settings.SolderHead2.MMPerJoint + settings.SolderHead2.SolderRetractDistance);
                }

                while (!_Solder1Feed.MotionComplete())
                {
                    Thread.Sleep(10);
                }
                while (!_Solder2Feed.MotionComplete())
                {
                    Thread.Sleep(10);
                } 

                if (backgroundWorkerSolderJob.CancellationPending)
                {
                    break;
                }
                RunSolderFast();
                if (settings.SolderHead1.HeadRunEnable)
                {
                    _Solder1Feed.StartRelativeMoveTo(0 - settings.SolderHead1.SolderRetractDistance);
                }
                if (settings.SolderHead2.HeadRunEnable)
                {
                    _Solder2Feed.StartRelativeMoveTo(0 - settings.SolderHead2.SolderRetractDistance);
                }
                
                
                while (!_Solder1Feed.MotionComplete())
                {
                    Thread.Sleep(10);
                }
                while (!_Solder2Feed.MotionComplete())
                {
                    Thread.Sleep(10);
                }
                RunSolderSlow();
                if (settings.SolderHead1.HeadRunEnable)
                {
                    currentSolder1Move = settings.SolderHead1.RetractDistance;
                }
                if (settings.SolderHead2.HeadRunEnable)
                {
                    currentSolder2Move = settings.SolderHead2.RetractDistance;
                }
                
                
                hasfinsihed = DoMove();

                
                while (!hasfinsihed)
                {
                    Thread.Sleep(10);
                }
                if (backgroundWorkerSolderJob.CancellationPending)
                {
                    break;
                }
                currentCaddy = currentCaddy + 2.54;
                
                hasfinsihed = DoMove();
                
                while (!hasfinsihed)
                {
                    Thread.Sleep(10);
                }
                if (backgroundWorkerSolderJob.CancellationPending)
                {
                    break;
                }
                
                currentrow++;

                
            }
            
           
            // cleaning cycle
           
            if (!backgroundWorkerSolderJob.CancellationPending)
            {
                CleaningCycle(CleaningLoc);
            }
            return true;
        }

        public void RetractSolder1()
        {
            RunSolderFast();
            _Solder1Feed.RelativeMoveTo(0 - settings.SolderHead1.SolderRetractDistance);
           
            while (!_Solder1Feed.MotionComplete())
            {
                Thread.Sleep(10);
            }
            RunSolderSlow();
        }
        public void RetractSolder2()
        {
            RunSolderFast();
            _Solder2Feed.RelativeMoveTo(0 - settings.SolderHead2.SolderRetractDistance);
           
            while (!_Solder2Feed.MotionComplete())
            {
                Thread.Sleep(10);
            }
            RunSolderSlow();
        }
        private bool CleaningCycle(int loc)
        {
            // clean solder tips in sponge 15mm past pcb edge

            bool hasfinsihed = false;
            currentSolder1Move = settings.SolderHead1.RetractDistance;
            currentSolder2Move = settings.SolderHead2.RetractDistance;
            hasfinsihed = DoMove();
            
            while (!hasfinsihed)
            {
                Thread.Sleep(10);
            }
           
            currentCaddy = dblCleanerLocations[loc];
          
            hasfinsihed = DoMove();
           
            while (!hasfinsihed)
            {
                Thread.Sleep(10);
            }
            for (int i = 1; i <= 5; i++)
            {
                currentSolder1Move = settings.SolderHead1.RetractDistance;
                currentSolder2Move = settings.SolderHead2.PCBHeight;
                hasfinsihed = DoMove();
               
                while (!hasfinsihed)
                {
                    Thread.Sleep(10);
                }
                currentSolder1Move = settings.SolderHead1.PCBHeight;
                currentSolder2Move = settings.SolderHead2.RetractDistance;
                hasfinsihed = DoMove();
               
                while (!hasfinsihed)
                {
                    Thread.Sleep(10);
                }
            }

            currentSolder1Move = settings.SolderHead1.RetractDistance;
            currentSolder2Move = settings.SolderHead2.RetractDistance;
            hasfinsihed = DoMove();

           
            while (!hasfinsihed)
            {
                Thread.Sleep(10);
            }
            return true;
        }
        private void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
           
        }


    }
}

