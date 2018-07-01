// Copyright 2018 xalanq, chang-ran
// License: LGPL v3.0

#include <boost/asio.hpp>

#include <core/Resource.h>
#include <core/SocketInfo.h>
#include <server/utils.h>

using boost::asio::ip::tcp;
boost::asio::io_service io_service;
tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 2333);

typedef X::xll xll;
typedef X::xint xint;
typedef X::ptree ptree;
typedef X::xstring xstring;
typedef X::ActionCode ActionCode;
typedef X::ErrorCode ErrorCode;

// for book operations
int main() {
    try {
        tcp::socket socket(io_service);
        xll token = 0;
        ptree pt;
        ActionCode ac = X::NoAction;
        Resource cover;

        cerr << "connect to " << ep.address() << ":" << 2333 << '\n';
        socket.connect(ep);
        
        pt.put("username", "PiRoot");
        pt.put("password", "2d515aaf7a77cb6744bf91a49af70928bfb9f04b");
        cerr << "Login send\n" << "token: " << token << "\n" << SocketInfo::encodePtree(pt, true) << '\n';

        X::tcp_sync_write(socket, 0, X::Login, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);

        cerr << "token: " << token << ", action_code: " << X::what(ac) << '\n' << SocketInfo::encodePtree(pt, true) << '\n';
        xll loginToken = token;

        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"奇点临近");
        pt.put("author", u8"雷·库兹韦尔");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    《奇点临近》是一本有思维方法论启示的书；是一本站在历史的高度，正面思考科技力量的书；是一本充满想象与预言，但又不失科学论证的书。\n    《奇点临近》提供了一个崭新的视角：21世纪既是数百年以来科技、创意的顶点，又是对人类终极命运真挚的愿景。");
        pt.put("position", u8"李文正图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/1/cover.jpg");
        pt.put("bookid", 1);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        
        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"C语言入门经典");
        pt.put("author", u8"刘志铭 杨丽");
        pt.put("ISBN", "1-234-56789-2");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    《C语言入门经典》面向C语言初学者，详细且全面地讲解了C语言的各种知识，从基础知识到高级应用，由浅入深，循序渐进地引领读者掌握C语言中的各种编程技术，体会C语言的灵活与强大。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/2/cover.jpg");
        pt.put("bookid", 2);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);

        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"Java入门经典");
        pt.put("author", u8"李伟,张金辉");
        pt.put("ISBN", "1-234-56789-3");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    本书在内容排列上由浅入深，让读者循序渐进地掌握编程技术；\n    在内容讲解上结合丰富的图解和形象的比喻，帮助读者理解“晦涩难懂”的技术；\n    在内容形式上附有大量的提示、技巧、说明等栏目，夯实读者编程技术，丰富编程经验。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/3/cover.jpg");
        pt.put("bookid", 3);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);

        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"菜鸟也能玩转Excel");
        pt.put("author", u8"巴蕾,王杉");
        pt.put("ISBN", "1-234-56789-4");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    主要针对Excel2010软件进行讲解，读者可以从零学起，逐渐掌握并精通各种操作。\n    《菜鸟也能玩转Excel：全面武装你的Excel操作能力》共7章。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/4/cover.jpg");
        pt.put("bookid", 4);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);

        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"走进互联网时代");
        pt.put("author", u8"胡舒立");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    过去的一年，实体经济遭遇到空前的困难，所有热钱几乎都流向互联网，“两会”是否对互联网经济有更大的支持力度，我们将从中得到哪些好处？马云、雷军、马化腾、周鸿祎、王石、俞敏洪，互联网与传统企业界大佬畅想互联网经济新时代！");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/5/cover.jpg");
        pt.put("bookid", 5);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);

        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"科学的极致：漫谈人工智能");
        pt.put("author", u8"集智俱乐部");
        pt.put("ISBN", "1-234-56789-5");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    《科学的极致：漫谈人工智能》由集智俱乐部成员共同创作而成，全面介绍了人工智能的历史及其在各个领域的发展及应用。\n    不仅涵盖了人机交互、脑科学、计算心理学、系统科学、社会科学等各个学科的基础理论，而且广泛讲述了人工智能在算法、软件、硬件等方面的应用以及跨学科应用。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/6/cover.jpg");
        pt.put("bookid", 6);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"知识的边界");
        pt.put("author", u8"温伯格·戴维");
        pt.put("ISBN", "1-234-56789-6");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    曾经，我们知道怎么去获取知识。我们的答案来自于书籍或者专家。我们会确定事实，继续前进。而在网络时代，知识已经进入了网络中，出现了史上最多的知识，但这些知识是不同的。所有确定性都被连根拔起，话题再无边界，没有人对任何事情能达成一致。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/7/cover.jpg");
        pt.put("bookid", 7);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"零基础学Python");
        pt.put("author", u8"张志强,赵越");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    Python是目前最流行的动态脚本语言之一。本书由浅入深，全面、系统地介绍了使用Python进行开发的各种知识和技巧。");
        pt.put("position", u8"李文正图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/8/cover.jpg");
        pt.put("bookid", 8);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"HTML5与CSS 3权威指南");
        pt.put("author", u8"陆凌牛");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    本书对HTML 5中新增的语法、标记方法、元素、API，以及这些元素与API到目前为止受到了哪些浏览器支持等进行详细介绍。在对它们进行介绍的同时将其与HTML 4中的各种元素与功能进行对比，以帮助读者更好地理解为什么需要使用HTML 5、使用HTML 5有什么好处、HTML 5中究竟增加了哪些目前HTML 4不具备而在第三代Web平台上将会起到重要作用的功能与API。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/9/cover.jpg");
        pt.put("bookid", 9);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"智能革命");
        pt.put("author", u8"李彦宏");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    人类历史上的历次技术革命，都带来了人类感知和认知能力的不断提升，从而使人类知道更多，做到更多，体验更多。\n    以此为标准，李彦宏在本书中将人工智能定义为堪比任何一次技术革命的伟大变革，并且明确提出，在技术与人的关系上，智能革命不同于前几次技术革命，不是人去适应机器，而是机器主动来学习和适应人类，并同人类一起学习和创新这个世界。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/10/cover.jpg");
        pt.put("bookid", 10);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"JAVA实例精通");
        pt.put("author", u8"李相国 ");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    《JAVA实例精通》共分为18章，循序渐进地讲述了Java SE 6.0的几乎所有知识点，从基础语法到核心技术、从面向对象思想到Java高级特性、从Java DB到MySQL数据库、从简单命令行到MyEclipse的具体操作都做了细致的讲解和演示，对所有知识点做了详尽的分析和字字珠玑的总结。");
        pt.put("position", u8"李文正图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/11/cover.jpg");
        pt.put("bookid", 11);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"互联网潜规则");
        pt.put("author", u8"赵斌");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    朝闻道，夕死可矣；一本书，看透网络世界明规则和潜规则；一本书，让你不至于被网络时代抛弃落伍；你想知道未来10年，20年，50年我们的网络生活的样子吗？成熟男人，女士的读物，互联网创业者必读。");
        pt.put("position", u8"李文正图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/12/cover.jpg");
        pt.put("bookid", 12);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"黑客简史");
        pt.put("author", u8"刘创");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"电子工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    本书主要以计算机发明之后的整个二十世纪后半叶世界上著名的黑客和黑客事件、以及计算机病毒等方面的解说，生动扫描整个黑客发展史。\n    其中包括去年轰动世界的斯诺登事件、Android手机之父安迪•鲁宾、阿桑奇事件以及号称“拥有键盘就会对世界千万威胁”的世界第一黑客凯文米特尼克和中国黑客联盟等精彩情节，其中贯穿着对黑客行为、道德、法律的反思和人文精神探讨。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/13/cover.jpg");
        pt.put("bookid", 13);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"一本书读懂大数据");
        pt.put("author", u8"章沛轩");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    大数据时代，我们需要学会用数据分析问题、解决问题。当下，因为没有数据的精确度量就无法进步。利用数据，我们可以看清、看准、看透所面对的个人问题，工作问题和所从事的商业问题。理解大数据、利用大数据是现代人标配的能力。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/14/cover.jpg");
        pt.put("bookid", 14);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"三体全集");
        pt.put("author", u8"刘慈欣");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    这是一部多重旋律的作品：此岸、彼岸与红岸，过去、现在与未来，交织成中国文学中罕见的复调。故事的核心是我们既熟悉又陌生的文革。\n    当主流文学渐渐远离了这个沉重的话题，大刘竟然以太空史诗的方式重返历史的现场，用光年的尺度来重新衡量那永远的伤痕，在超越性的视野上审视苦难、救赎与背叛。");
        pt.put("position", u8"李文正图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/15/cover.jpg");
        pt.put("bookid", 15);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        
        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"时间移民");
        pt.put("author", u8"刘慈欣");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    本书是刘慈欣2014年出版的最新作品，这是刘慈欣二十余年创作生涯的精华之作。\n    在本书中，刘慈欣构建了未来世界人类社会最神奇的可能性。\n    故事讲述了由于环境恶化和人口压力，地球政府决定派出远征队伍，选取25岁以下的人类成员向未来移民。\n    移民队伍进行了多次停留，但每一次的停留环境都不再适合人类居住，最后一次航程，“移民大使”把时间锁定在未来11000年。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/16/cover.jpg");
        pt.put("bookid", 16);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"80天环游地球");
        pt.put("author", u8"凡尔纳");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    《80天环游地球》是凡尔纳一部引人入胜的小说，笔调生动活泼，富有幽默感。\n    小说叙述了英国人福克先生因和朋友打赌，而在80天内克服重重困难完成环游地球一周的壮举。\n    书中不仅详细描写了福克先生一行在途中的种种离奇经历和他们所遇到的千难万险，还使人物的性格逐渐立体化，如沉默寡言、机智、勇敢、充满人道精神的福克，活泼好动易冲动的仆人“路路通”等等");
        pt.put("position", u8"李文正图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/17/cover.jpg");
        pt.put("bookid", 17);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"异形");
        pt.put("author", u8"蒂姆·利本,李升升");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    克里斯·霍伯还是个孩子的时候就梦见过怪物。但在深邃的外太空，他感受到的只有无边的黑暗和孤寂。\n    后来，在LV178星上，他和他的矿工朋友们发现这里是一个地狱般的存在，星球表面尽是暴风雨侵蚀的痕迹，岩石被风暴摧残得斑驳陆离——特莫耐特矿石，这一人类已知蕞坚硬的物质就埋藏在这颗星球的地下。\n    一艘航天穿梭机撞上了采矿飞船马里昂号，随后发生的一切，让矿工们意识到在洞穴的深处远非只有特莫耐特矿石这么简单。");
        pt.put("position", u8"李文正图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/18/cover.jpg");
        pt.put("bookid", 18);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"重启人");
        pt.put("author", u8"艾米·亭特拉");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    五年前，十二岁的女孩瑞恩·康纳利胸口中了三枪；178分钟后，她在太平间醒来，成为一个重启人，代号一七八。\n    由于她是死亡时间最长的人，所以也是HARC机构里速度、力量、自愈力最强的。她执行任务效率惊人，冷酷无情。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/19/cover.jpg");
        pt.put("bookid", 19);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        
        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"地心游记");
        pt.put("author", u8"凡尔纳");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    《地心游记》发表于1864年，是凡尔纳早期著名的科幻小说之一。\n    小说讲述的是德国科学家里登布洛克教授前人一封密码信的启发，偕同侄子阿克塞尔和向导汉斯，进行了一次穿越地心的探险旅行。他们从冰岛的斯奈菲尔火山口下降，在地心经历三个月的艰辛跋涉，一路克服缺水、迷路、暴风雨等艰难险阻，最后从西西里岛的火山口返回地面。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/20/cover.jpg");
        pt.put("bookid", 20);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"海底两万里");
        pt.put("author", u8"儒勒·凡尔纳");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    神秘“海怪”频频袭击各国海轮。阿龙纳斯教授等人在追捕“海怪”时落水，进入“海怪”之巾。“海怪”究竟是什么？他们能求得生存吗？尼摩船长的出现给出了答案。他带着教授等人开始了海底探险之旅，一起穿越美丽的海底森林和珊瑚王国，探访海底废墟和煤矿，打捞沉船宝藏，还问凶猛的抹香鲸、恐怖的大章鱼展开了搏斗。而在此过程中，尼摩船长身上神秘、危险的气息也越来越浓……");
        pt.put("position", u8"李文正图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/21/cover.jpg");
        pt.put("bookid", 21);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        
        
        pt = ptree();
        pt.put("bookid", 0);
        pt.put("title", u8"无人生还");
        pt.put("author", u8"阿加莎•克里斯蒂");
        pt.put("ISBN", "1-234-56789-1");
        pt.put("publisher", u8"机械工业出版社");
        pt.put("amount", 233);
        pt.put("introduction", u8"    十个相互陌生、身份各异的人受邀前往德文郡海岸边一座孤岛上的豪宅。客人到齐后，主人却没有出现。\n    当晚，一个神秘的声音发出指控，分别说出每个人心中罪恶的秘密。接着，一位客人离奇死亡。暴风雨让小岛与世隔绝，《十个小士兵》——这首古老的童谣成了死亡咒语。如同歌谣中所预言的，客人一个接一个死去……杀人游戏结束后，竟无一人生还！\n    开创童谣杀人和孤岛模式，古典推理的不朽名作。");
        pt.put("position", u8"逸夫图书馆");
        pt.put("priority", 1);
        pt.put("maxKeepTime", 604800);
        
        X::tcp_sync_write(socket, token, X::SetBook, pt);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        pt = ptree();
        cover = Resource::get("book/22/cover.jpg");
        pt.put("bookid", 22);
        pt.put("fileSize", cover.getSize());
        X::tcp_sync_write_with_file(socket, token, X::SetBookCover, pt, cover);
        pt = ptree();
        X::tcp_sync_read(socket, token, ac, pt);
        

        socket.close();

    } catch (std::exception &e) {
        cerr << e.what() << '\n';
    }
    return 0;
}