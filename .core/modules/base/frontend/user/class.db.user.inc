<?php
/**
 * ============================================================
 * File name: class.db.category.inc
  *   Purpose:
 * ============================================================
 */
	class DbBaseUser extends DB
	{
		function __construct()
		{
			parent::__construct();
			load_config ( dirname(__FILE__).DIRECTORY_SEPARATOR."conf.inc" );
			$this->table = 'base_user';
			//$this->fields = include('fields.inc');
		}
/*
		function get_extra_fields()
		{
			return include('fields_extension.inc');
		}

		function check_reply_username($username)
		{
			$username = str_replace('"', '', $username);
			$sql = 'SELECT
					*
				FROM
					`'.$this->table.'`
				WHERE
					`login` = "'.$username.'"
				LIMIT 0, 1';
			$this->query($sql);

			if ($this->get_num_rows () > 0)return false;
			else  return true;
		}

		function check_reply_email($email)
		{
      			$email = str_replace('"', '', $email);
			$sql = 'SELECT
					*
				FROM
					`'.$this->table.'`
				WHERE
					`email` = "'.$email.'"
				LIMIT 0, 1';

			$this->query($sql);

			if($this->get_num_rows () > 0)return false;
			else return true;
		}

		function GetCountriesList()
		{
			$sql = "SELECT * FROM `generic_country` 
						ORDER BY `popularity` DESC, `country_name` ASC
				";
			$this->query($sql);
			$result = array();
			$result[DEFAULT_SELECT_VAL] = DEFAULT_SELECT_NAME;
			while($row = $this->fetch_row())
			{
				$result[$row['id']] = $row['country_name'];
			}
			//print_r($result);
			return $result;
		}
		function is_email_occupied ($email, $current_email = "")
		{
			$table = 'base_user';
			$sql = "
				SELECT * FROM
					`".$table."`
				WHERE
					`email` = '".$email."' 
				";
			// ���� ���� ���_���� �� ��������� ��� �� ������
			if ($current_email !== ""){
				$sql_plus = " AND `email` <> '".$current_email."'";
				$sql .= $sql_plus;
			}
			//echo "<h2>$sql</h2>";
			$this->query($sql);
			$result = array();
			while($row = $this->fetch_row())
			{
				$result[]= $row;
			}
			// esli ���� ����� ���� � ����� �� ����� ���������� ���
			if ( !empty ($result) ){
				//print_r($result);
				return true;
			}else{
				return false;
			}
		}

		function MarkUserAsVoted($uid)
		{
			$sql = "UPDATE `base_user`
					SET `has_voted` = 'yes'
					WHERE
						`id` = '".$uid."' 
				";
			//echo "<h2>$sql</h2>";
			$this->query($sql);
		}

		function PushComment($writer, $receiver, $comment)
		{
			$sql = "INSERT INTO `vote_comments` (`writer_id`, `receiver_id`, `comment`, `date`)
						VALUES ('$writer', '$receiver', '$comment', '".time()."')
			";
			$this->query($sql);
		}

		function GetUserComments($receiver_id)
		{
			$sql = "SELECT * FROM `vote_comments` WHERE `receiver_id`='$receiver_id' AND `receiver_id` > 0 AND `writer_id`>0 ORDER BY `date` ASC";
			$this->query($sql);
			$result = array();
			while($row = $this->fetch_row())
			{
				$result[] = $row;
			}
			//print_r($result);
			return $result;
		}
		function direct_user_base($uid)	
		{
			$sql = "SELECT * FROM `base_user` WHERE `id` = '$uid'";
			$this->query($sql);
			$row = $this->fetch_row();
			return $row;
		}

		function direct_user_extra($uid)	
		{
			$sql = "SELECT * FROM `type_user1` WHERE `user_id` = '$uid'";
			$this->query($sql);
			$row = $this->fetch_row();
			return $row;
		}

		function GetTotalRegisteredUsers()
		{
			$sql = "SELECT COUNT( * ) as total FROM `base_user`
					WHERE type_id =2 AND `paid` = 'yes' ";
			$this->query($sql);
 			$row = $this->fetch_row();
			return $row['total'];
		}


		function TextIdExists($text_id, $uid = 0)
		{
			$ANDSTR = "";
			if ($uid > 0){
				$ANDSTR = " AND `id` <> $uid ";
			}
			$sql = "SELECT  count(*) as `count` FROM `base_user` WHERE `text_id` = '$text_id' $ANDSTR";
			$this->query($sql);
			$row = $this->fetch_row();
			if ($row['count'] == 0){
				return false;
			}else{
				return true;
			}
		}
		
		function GenereateUserTextId($login, $uid = 0)
		{
			$replace_pattern = "/\!|\@|\#|\\$|\%|\^|\&|\*|\+|\\\|\{|\}|\<|\>|\?|\'|\/|\.|\ |\,|\~|\)|\(|\]|\[|\=|\:|\�|\�/";
			$masked = translit($login);
			$masked = preg_replace ($replace_pattern, "_", $masked);
			$masked = strtolower($masked);
			//echo "<h1>$masked</h1>";
			//$result[$k]['text_id'] [0] = strtoupper($val['text_id'] [0] );
			$text_id = $masked;
		
			///uniqueness
			$i = 0;			
			while ( $this->TextIdExists($text_id, $uid) === true ){
				$len_i = strlen($i);
				$i++;
				if ($i>1){
					$text_id = substr($text_id, 0, $len_i*(-1));
				}
				$text_id .= $i;
			} 
			return $text_id;
		}

		function GetUIDByTextId($text_id)
		{
			$sql = "SELECT * FROM `base_user`
					WHERE text_id = '$text_id'";
			//echo "<h1>SQL = $sql</h1>";
			$this->query($sql);
			$row = $this->fetch_row();
			return $row['id'];
		}
*/

		
	}

?>
