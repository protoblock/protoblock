using System;
using NNanomsg;
using System.IO;
using NNanomsg.Protocols;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using TweetSharp;



namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            string ConsumerKey = " ";
            string ConsumerSecret = " ";
            string AccessToken = " ";
            string AccessTokenSecret = " ";

            string TwitterLogin = "TF_Ticker";
            if (TwitterLogin == "newberg_test")
            {
                #region newberg_test
                ConsumerKey = "QYqHNChjRxHlswbYUESHhw";
                ConsumerSecret = "ZkOrsTTU1GmnaQBSt5jzunMyNTXfrX4nAMMYxIkKSk";
                AccessToken = "127980366-qBKxmO3F1XAgl4mZ8rApsZuY7jt4b6ms0FMxwE5R";
                AccessTokenSecret = "dVyh1YwpaWkXMmEACCOdBTRP8sEyVabyyo5HofgHBlE";
                #endregion
            }
            else if (TwitterLogin == "TF_Ticker")
            {
                #region newberg_alpha
                ConsumerKey = "Kkuh8WBi5O2sTIlRd7XALiNjQ";
                ConsumerSecret = "7OCwZV9zd58mioyESDewf9TDlsJGUu8niuJYfZRM2dTro51RI5";
                AccessToken = "3305320862-vktlFL7JYwH8bqsIeQdPyV0FGdRdExTyRcvxyqr";
                AccessTokenSecret = "HoSKMd5ikQTag1C7drMu72SXoENg3BA9tyBrqrNtFiVxr";
                #endregion
            }

            TwitterService mTwitterService;
            mTwitterService = new TwitterService(ConsumerKey, ConsumerSecret);
            mTwitterService.AuthenticateWith(AccessToken, AccessTokenSecret);
           /// string str = "@" + TwitterLogin + " test " + System.DateTime.Now.ToUniversalTime().ToString("HH:mm:ss");

    
            var s = new PairSocket();
            const string socketAddress = "tcp://127.0.0.1:5088";
            s.Connect(socketAddress);
            while (true)
            {
                var data = s.Receive();
                if (data != null)
                {
                    string str = Encoding.UTF8.GetString(data);
                    Console.WriteLine("RECEIVED: '" + Encoding.UTF8.GetString(data) + "'");

                    try
                    {
                        mTwitterService.SendTweet(new SendTweetOptions { Status = str });
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("<b>" + ex.Message + "</b><br>" + ex.StackTrace);
                    }

                }
            }

        }
   
        
    }
}
