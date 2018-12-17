//
// Created by cmm on 2018/12/13.
//



class Surf{
public:

    static void imageToVideo(const string &imagePath, const string &videoPath){
        char image_name[15];
        string s_image_name;
        VideoWriter writer;
        int isColor = 1;
        int frame_fps = 30;
        int frame_width = 320;
        int frame_height = 240;
        string video_name = videoPath+"out.avi";
        writer = VideoWriter(video_name, CV_FOURCC('X', 'V', 'I', 'D'),frame_fps,Size(frame_width,frame_height),isColor);
        cout << "frame_width is " << frame_width << endl;
        cout << "frame_height is " << frame_height << endl;
        cout << "frame_fps is " << frame_fps << endl;
        //namedWindow("image to video", CV_WINDOW_AUTOSIZE);
        int num = 90;//输入的图片总张数
        int i = 0;
        Mat img;
        while (i<=num)
        {
            sprintf(image_name, "%d%s",  ++i, ".jpg");
            s_image_name = path_scr+image_name;
            img = imread(s_image_name);//读入图片
            if (!img.data)//判断图片调入是否成功
            {
                cout << "Could not load image file...\n" << endl;
            }
            //imshow("image to video",img);
            //写入
            writer.write(img);
        }
    };

};
