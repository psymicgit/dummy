<?php
class mysqlclass {
    var $link;
    var $querynum = 0;
    
    // 
    function connect($dbhost, $dbuser, $dbpwd, $dbname, $pconnect = 0) {
        if($pconnect) {
            if(!$this->link = @mysql_pconnect($dbhost, $dbuser, $dbpwd)) {
                $this->mysql_errormsg('Can not connect to MySQL server');
            }
        } else {
            if(!$this->link = @mysql_connect($dbhost, $dbuser, $dbpwd)) {
                $this->mysql_errormsg('Can not connect to MySQL server');
            }
        }
        
        @mysql_query("set names 'utf8'");
        @mysql_select_db($dbname, $this->link);
    }

    // 
    function select_db($dbname) {
        return @mysql_select_db($dbname, $this->link);
    }

    // 
    function query($sql) {    
        $sql = str_replace("INSERT ", "INSERT IGNORE ", $sql);
        $sql = str_replace("insert ", "INSERT IGNORE ", $sql);
        $sql = str_replace("update ", "update IGNORE ", $sql);
        $sql = str_replace("UPDATE ", "UPDATE IGNORE ", $sql);    

        if(!($query_result = @mysql_query($sql, $this->link))) {
            $this->mysql_errormsg('MySQL Query Error', $sql);
        }        

        $this->querynum++;        

        return $query_result;
    }

    // 
    function fetch_array($query_result, $result_type = MYSQL_ASSOC) {
        return @mysql_fetch_array($query_result, $result_type);
    }

    // 
    function fetch_row($query_result) {
        $row = @mysql_fetch_row($query_result);
        return $row;
    }
    
    // 
    function result($query_result, $row, $field) {
        $cell = @mysql_result($query_result, $row, $field);//
        return $cell; 
    }       

    // 
    function num_rows($query_result) {
        $row_cnt = @mysql_num_rows($query_result);
        return $row_cnt;
    }
    
    // 
    function num_fields($query_result) {
        return @mysql_num_fields($query_result);
    }     

    // 
    function insert_id() {
        $id = @mysql_insert_id();
        return $id;
    }
    
    // 
    function affected_rows() {
        return @mysql_affected_rows();    
    }    

    // 
    function free_result($query_result) {
        return @mysql_free_result($query_result);
    }
    
    // 
    function mysql_errormsg($message = '', $sql = '') {
        echo $message."<br>".$sql."<br>";
        echo $this->errno()." ".$this->error();
        exit;
    }

    // 
    function error() {
        return @mysql_error();
    }

    // 
    function errno() {
        return @intval(mysql_errno());
    }
    
    //  
    function close() {
        return @mysql_close();
    }    
}
?>
