var http = require('http');
// 引入axios库  
const axios = require('axios');  
  
// 使用axios发起GET请求，这里以获取JSONPlaceholder的示例数据为例  
var data;
axios.get('https://jsonplaceholder.typicode.com/posts/1')  
  .then(function (response) {  
    // 请求成功时执行，response.data是返回的数据 
	data = 	response.data;
    console.log(response.data);  
  })  
  .catch(function (error) {  
    // 请求失败时执行  
	data = error;
    console.log(error);  
  }).finally(()=>{
    const prettyString = JSON.stringify(data, null, 2);
      var versions_server = http.createServer((request, response) =>
    { response.end(prettyString); });
    versions_server.listen(3000);
  })
  

