using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SolderingRobotController
{
    public class AppConfig
    {
       

        public AppSettings SetDefaultConfig()
        {
            AppSettings settings = new AppSettings();
            SolderHeadSettings solderhead1 = new SolderHeadSettings();
            SolderHeadSettings solderhead2 = new SolderHeadSettings();

            solderhead1.IronTargetTemperature = 380;
            solderhead1.IronTargetTemperature = 380;
            settings.PinsToSolder = 40;
            settings.PCBLocation1 = true;
            settings.PCBLocation2 = true;
            settings.PCBLocation3 = true;
            settings.PCBLocation4 = true;
            settings.PCBLocation5 = true;

            // setup solder head 1

            solderhead1.HeadRunEnable = true;
            solderhead1.MMPerJoint = 8;
            solderhead1.HeatingTime = 100;
            solderhead1.LoadSolderDistance = 300;
            solderhead1.RetractDistance = 10;
            solderhead1.PCBHeight = 26;
            solderhead1.SolderRetractDistance = 8;

            solderhead1.HeadCPM = 225.0;
            solderhead1.HeadVelocity = 4e8;
            solderhead1.HeadAcceleration = 4e5;
            solderhead1.HeadJerk = 4e6;

            solderhead1.FeedCPM = 193.0;
            solderhead1.FeedVelocity = 2e3;
            solderhead1.FeedAcceleration = 2e3;
            solderhead1.FeedJerk = 4e6;

            solderhead1.LoadingFeedVelocity = 4e3;
            solderhead1.LoadingFeedAcceleration = 5e3;
            solderhead1.LoadingFeedJerk =  4e6;

            // setup solder head 2
            solderhead2.HeadRunEnable = true;
            solderhead2.MMPerJoint = 8;
            solderhead2.HeatingTime = 100;
            solderhead2.LoadSolderDistance = 300;
            solderhead2.RetractDistance = 10;
            solderhead2.PCBHeight = 26;
            solderhead2.SolderRetractDistance = 8;

            solderhead2.HeadCPM = 233.0;
            solderhead2.HeadVelocity = 4e8;
            solderhead2.HeadAcceleration = 4e5;
            solderhead2.HeadJerk = 4e6;

            solderhead2.FeedCPM = 193.0;
            solderhead2.FeedVelocity = 2e3;
            solderhead2.FeedAcceleration = 2e3;
            solderhead2.FeedJerk = 4e6;

            solderhead2.LoadingFeedVelocity = 4e3;
            solderhead2.LoadingFeedAcceleration = 5e3;
            solderhead2.LoadingFeedJerk = 4e6;

            settings.SolderHead1 = solderhead1;
            settings.SolderHead2 = solderhead2;


            return settings;
        }

     
    }
}
