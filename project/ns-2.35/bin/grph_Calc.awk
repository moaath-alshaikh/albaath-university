BEGIN {
	recvdSize = 0
	startTime = 0
	stopTime = 0
	sent=0
	receive=0
    agtCount=0
    rtrCount=0


    startCount=0
    endCount=0
}

{
    event = $1
  time = $2
  node_id = $3
  pkt_size = $8
  level = $4

  # شرط يحسب عدد رزم التوجيه المرسلة 
  #  AGT حيث أن 
  # تعني أن الرزمة تخص طبقة النقل 
  if (( $1 == "r") && ( $7 == "cbr" || $7 =="tcp" ) && ( $4=="AGT" ))  agtCount++;

  
  # شرط يحسب عدد رزم البيانات التي تم استقبالها بنجاح 
  #  RTR حيث أن 
  # تعني أن الرزمة خاصة ببروتوكولات التوجيه 
  if (($1 == "s") && $4 == "RTR") rtrCount++;
 
 
 #شرط يحسب الرزم المرسلة 
  if (level == "AGT" && event == "s" && $7 == "cbr") {
    sent++;
    if (!startTime || (time < startTime)) {
      startTime = time
    }
    startTimeArr[startCount] = $2;
    startCount++;
  }

  #شرط يحسب الرزم المستقبلة 
  if (level == "AGT" && event == "r" && $7 == "cbr") {
    receive++;
    if (time > stopTime) {
      stopTime = time
    }
    recvdSize += pkt_size
    endTimeArr[endCount] = $2;
    endCount++;
  }

  if (level == "AGT" && event == "d" && $7 == "cbr") {
    endTimeArr[$35] = -1;
  }

}

END {
# عداد يحسب عدد رزم البيانات الواصلة للعقد الهدف 
  count = 0
  for(i=0;i<startCount;i++) {
    if(endTimeArr[i] > 0){
        delay[i] = endTimeArr[i] - startTimeArr[i];
        count++;
    }
    else
    {
        delay[i] = -1;
    }
  }

  # يحسب الزمن الكلي لوصول جميع رزم البيانات من العقد المصدر إلى العقد الهدف 
  endToEndDelay = 0;
  for(i=0; i<count; i++) {
   if(delay[i] > 0) {
    endToEndDelay = endToEndDelay + delay[i];
   }
  }
  #قانون زمن التأخير الكلي 
  endToEndDelay = endToEndDelay/count;
  
  printf("Sent\t %d\n",sent)
  printf("Received %d\n",receive)
  printf("Dropped %d\n",sent-receive)  
  printf("PDR %.2f\n",(receive/sent)*100);
  printf("Normalized Load\t %0.3f\n",((agtCount*1.0)/rtrCount));
  print "Average End-to-End Delay = " endToEndDelay * 1000 " ms";

}

#1
# قانون معدل تأخير رزم البيانات الكلي هو 
# الزمن الكلي اللازم لوصول جميع رزم البيانات من العقد المصدر إلى العقد الهدف 
# على (\) عدد رزم البيانات الواصلة للعقدة الهدف 

#2
# قانون نسبة تسليم رزم البيانات (%) هو
# عدد رزم البيانات المستقبلة  \عدد رزم البيانات المرسلة) مضروبا ب100 من أجل النسبة المئوية)

#3
# قانون الحمل الناتج عن رزم التوجيه
# عدد رزم التوجيه المرسلة \ عدد رزم البيانات التي تم استقبالها بنجاح) ا)


