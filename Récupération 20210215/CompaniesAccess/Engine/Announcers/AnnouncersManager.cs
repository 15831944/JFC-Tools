using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AR7Common;
using RadioEngine;

namespace CompaniesAccess
{
    public class AnnouncersManager : IUserDataManager
    {
        private IAnnouncerDataSource m_source;

        public CompaniesFileData Files { get; private set; }
        public DimensionLinked Link { get; private set; }

        public AnnouncersManager()
        {
            this.Files = new CompaniesFileData();
            this.Link = new DimensionLinked(new String[] { AnnouncerUsersDimensionCreator.AnnouncerNames[0], AnnouncerUsersDimensionCreator.AnnouncerNames[1] });

            // the source is the Consolidation database
            this.m_source = new ConsoDatabaseAnnouncers();
        }

        public ADimensionCreator DimensionCreator
        {
            get
            {
                return new AnnouncerUsersDimensionCreator(this.Files, this.m_source);
            }
        }


        public List<RangeIndexModalities> Ranges
        {
            get
            {
                List<RangeIndexModalities> l_list = new List<RangeIndexModalities>();

                this.Link.Initialize();
                if (this.Link.IsInitialized)
                {
                    // for all the users
                    foreach (KeyValuePair<AModalityIndex, BaseModality> i_userModality in this.Link.Dimensions[1].AllModalities)
                    {
                        String l_currentUser = i_userModality.Value.Caption;
                        if (this.Files.Files.ContainsKey(l_currentUser))
                        {
                            RangeIndexModalities l_currentRangeUser = new RangeIndexModalities();

                            // for all the announcers
                            List<AModalityIndex> l_currentAnnouncers = new List<AModalityIndex>();
                            foreach (KeyValuePair<AModalityIndex, BaseModality> i_announcerModality in this.Link.Dimensions[0].AllModalities)
                            {
                                // check if the user has the rights for the announcer
                                if (this.m_source.AnnouncerIsValid(i_userModality.Value.Caption, i_announcerModality.Value.Caption))
                                    l_currentAnnouncers.Add(i_announcerModality.Key);
                            }

                            if (l_currentAnnouncers.Count > 0)
                            {
                                l_currentRangeUser.AddValue(this.Link.IndexesOfDimension[1], i_userModality.Key);
                                l_currentRangeUser.AddRangeValue(this.Link.IndexesOfDimension[0], l_currentAnnouncers);
                                l_list.Add(l_currentRangeUser);
                            }
                        }
                    }
                }

                return l_list;
            }
        }

        public bool CanOnlySelect
        {
            get { return false; }
        }

        public bool SelectAllOnHeaderClick
        {
            get { return true; }
        }

        public bool NewModalityAvailable
        {
            get { return false; }
        }

        public bool IsValidForUser(IndexModalities _indexes)
        {
            return true;
        }

        public void SaveRanges(List<RangeIndexModalities> _ranges)
        {
            if (this.Link.IsInitialized)
            {
                List<Tuple<String, String>> l_rights = new List<Tuple<String, String>>();

                foreach (RangeIndexModalities i_range in _ranges)
                {
                    // the user
                    if (i_range.RangeIndexes.ContainsKey(this.Link.IndexesOfDimension[1]))
                    {
                        AModalityIndex l_userIndex = i_range.RangeIndexes[this.Link.IndexesOfDimension[1]].FirstOrDefault();
                        if (l_userIndex != null)
                        {
                            String l_user = this.Link.Dimensions[1].AllModalities[l_userIndex].Caption;

                            if (this.Files.Files.ContainsKey(l_user))
                            {
                                // the announcers
                                if (i_range.RangeIndexes.ContainsKey(this.Link.IndexesOfDimension[0]))
                                {
                                    foreach (AModalityIndex l_announcerIndex in i_range.RangeIndexes[this.Link.IndexesOfDimension[0]])
                                    {
                                        String l_announcer = this.Link.Dimensions[0].AllModalities[l_announcerIndex].Caption;
                                        l_rights.Add(new Tuple<String, String>(l_user, l_announcer));
                                    }
                                }
                            }
                        }
                    }
                }

                // updates the announcers rights
                this.m_source.UpdateRights(l_rights);
            }
        }

        public void ClearDimensions()
        {
            DimensionManager.Singleton.RemoveDimension("temp");
            DimensionManager.Singleton.RemoveDimension(AnnouncerUsersDimensionCreator.AnnouncerNames[0]);
            DimensionManager.Singleton.RemoveDimension(AnnouncerUsersDimensionCreator.AnnouncerNames[1]);
        }
    }
}
