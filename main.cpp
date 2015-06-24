#include <opencv2/opencv.hpp>
#define count_file 4


int fase_recognition(const cv::Mat &src, cv::CascadeClassifier &haar_cascad, cv::Mat &fase)
{
	cv::Size size_image = cv::Size(src.cols+1, src.rows+1);

        cv::Mat tmp;

        std::vector<cv::Rect> vector_objekt;

        // Уменьшаем разрешение изображения на 50%
        cv::resize(src, tmp, cv::Size(), 0.5, 0.5);
        // Делаем изображение чернобелым
        cv::cvtColor(tmp, tmp, CV_BGR2GRAY);
        // выравниваем гистограмму изображения так как в условиях низкой освешенности алгоритм распознания
        // работает не эффективно.
        for (int i = 0; i < 3; i++)
            cv::equalizeHist(tmp, tmp);

        // Засекаем время необходимое для распознания
        double t = (double)cvGetTickCount();

        haar_cascad.detectMultiScale(tmp, vector_objekt, 1.3, 3, CV_HAAR_DO_CANNY_PRUNING, cv::Size(25,25));

        t = (double)cvGetTickCount() - t;

        

       // Если лицо обнаружено, то vector_objekt.size() = 1, если нет то vector_objek.size = 0

        for (ushort i = 0; i < vector_objekt.size(); i++)
        {
            vector_objekt[i].x = vector_objekt[i].x * 2;
            vector_objekt[i].y = vector_objekt[i].y * 2;

            vector_objekt[i].width = vector_objekt[i].width * 2;
            vector_objekt[i].height = vector_objekt[i].height * 2;
            // печатаем значение времени затраченное на распознание
	    std::cout << "detect time = " << t/((double)cvGetTickFrequency()*1000.) << std::endl;
       	    fase = src(vector_objekt[i]);
	    return 1;
            
        }
	return 0;

}


void save_fase(cv::Mat frame, int &z_foto)
{

    std::stringstream ss;
    ss << z_foto;
    std::string st = "tmp/" + ss.str() + ".jpg";
    // делаем изображение чернобелым
    cv::cvtColor(frame, frame, CV_BGR2GRAY);
    //cv::equalizeHist(frame, frame);
    cv::imwrite(st, frame);

    std::cout << "Save: " << st << std::endl;

}


int open_img(cv::Mat &src, int &z_foto)
{
	std::string str = "";
	try
{
	std::stringstream stream;
	stream << z_foto;
	str = stream.str() + ".tiff";
	src = cv::imread(str);

	if (src.data)
	{
		std::cout << "open file: " << str << std::endl;
		return 1;
	}
	
	return 0;
}
	catch (cv::Exception c)
{
	std::cout << "error open file: " << str << std::endl;
	return 0;	
}	
	
}

int main()
{

	std::string ClassficatorFileName = "./LBPCASCADE/lbpcascade_frontalface.xml";
	cv::CascadeClassifier haar_cascad;
	haar_cascad.load(ClassficatorFileName);

	for (int i = 1; i <= count_file; i++)
	{
		cv::Mat tmp;
		cv::Mat fase;
		// Если изображение не открыли пропускаем интерацию цикла
		if (!open_img(tmp, i))
		continue;
		
		// Если лицо не нашли пропускаем интерацию цикла
		if (!fase_recognition(tmp, haar_cascad, fase))
		{			
			std::cout << "no fase detect!" << std::endl;
			continue; 

		}
		save_fase(fase, i);
		
	}


	return 0;
}
