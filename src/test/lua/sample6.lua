function ThreadTest()
	print("ThreadTest 시작")
	
	print("TestFunc 호출")
	TestFunc()
	TestFunc2(1.2)
	print("TestFunc 종료")
	
	print("g_test::TestFunc() 호출")
	g_test:TestFunc()
	g_test:TestFunc2(2.3)
	print("g_test::TestFunc() 종료")
	
	print("ThreadTest 종료")
end
