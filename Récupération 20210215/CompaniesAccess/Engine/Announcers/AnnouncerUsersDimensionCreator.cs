using AR7Common;
using ARDataProvider;
using RadioEngine;
using System;
using System.Collections.Generic;

namespace CompaniesAccess
{
    public class AnnouncerUsersDimensionCreator : ADimensionCreator
    {
        static public String[] AnnouncerNames = { "announcer", "user" };

        private CompaniesFileData Files { get; set; }

        private IAnnouncerDataSource Source { get; set; }

        public AnnouncerUsersDimensionCreator(CompaniesFileData _files, IAnnouncerDataSource _source)
        {
            this.Files = _files;
            this.Source = _source;
        }

        protected override void CreateDimension()
        {
            if (this.Source != null)
            {
                this.Source.Initialize();
                this.Files.Initialize();

                Dimension l_dimAnnouncer = new Dimension(AnnouncerUsersDimensionCreator.AnnouncerNames[0], "Nom dim");  // A TRADUIRE DIMENSION
                Dimension l_dimUser = new Dimension(AnnouncerUsersDimensionCreator.AnnouncerNames[1], "Nom dim");       // A TRADUIRE DIMENSION
                foreach (KeyValuePair<String, ARUserFileTable> i_userFile in this.Files.Files)
                {
                    BaseModality l_currentUser = new BaseModality(i_userFile.Key, Dimension.EMPTY_VALUE);
                    l_dimUser.AddModality(l_currentUser);

                    int l_index = 0;
                    IEnumerable<String> l_announcers = this.Source.Announcers;
                    if (l_announcers != null)
                    {
                        foreach (String l_announcer in l_announcers)
                        {
                            BaseModality l_currentAnnouncer = new BaseModality(l_announcer, l_index++);
                            l_dimAnnouncer.AddModality(l_currentAnnouncer);
                        }
                    }
                }


                this.Dimensions.Add(l_dimAnnouncer);
                this.Dimensions.Add(l_dimUser);
            }
        }
    }
}
