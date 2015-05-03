using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SolderingRobotController
{
    public class AppSettings
    {
        public int PinsToSolder { get; set; }
        public bool PCBLocation1 { get; set; }
        public bool PCBLocation2 { get; set; }
        public bool PCBLocation3 { get; set; }
        public bool PCBLocation4 { get; set; }
        public bool PCBLocation5 { get; set; }
        public SolderHeadSettings SolderHead1 { get; set; }
        public SolderHeadSettings SolderHead2 { get; set; }
    }

    public class SolderHeadSettings
    {
        public bool HeadRunEnable { get; set; }
        public double IronTargetTemperature { get; set; }
        public double MMPerJoint { get; set; }
        public double HeatingTime { get; set; }
        public double LoadSolderDistance { get; set; }
        public double RetractDistance { get; set; }
        public double PCBHeight { get; set; }
        public double SolderRetractDistance { get; set; }

        public double HeadCPM { get; set; }
        public double HeadVelocity { get; set; }
        public double HeadAcceleration { get; set; }
        public double HeadJerk{ get; set; }

        public double FeedCPM { get; set; }
        public double FeedVelocity { get; set; }
        public double FeedAcceleration { get; set; }
        public double FeedJerk { get; set; }

        public double LoadingFeedVelocity { get; set; }
        public double LoadingFeedAcceleration { get; set; }
        public double LoadingFeedJerk { get; set; }
    }




}
