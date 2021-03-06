//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from: game_db.proto
namespace Msg.Protocol
{
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"SqlMsg")]
  public partial class SqlMsg : global::ProtoBuf.IExtensible
  {
    public SqlMsg() {}
    
    private Msg.Protocol.SqlType _sqltype;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"sqltype", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public Msg.Protocol.SqlType sqltype
    {
      get { return _sqltype; }
      set { _sqltype = value; }
    }
    private ulong _sqlid;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"sqlid", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public ulong sqlid
    {
      get { return _sqlid; }
      set { _sqlid = value; }
    }
    private string _sql;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"sql", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public string sql
    {
      get { return _sql; }
      set { _sql = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"PostSql")]
  public partial class PostSql : global::ProtoBuf.IExtensible
  {
    public PostSql() {}
    
    private uint _time;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"time", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public uint time
    {
      get { return _time; }
      set { _time = value; }
    }
    private Msg.Protocol.SqlDBType _dbtype;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"dbtype", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public Msg.Protocol.SqlDBType dbtype
    {
      get { return _dbtype; }
      set { _dbtype = value; }
    }
    private readonly global::System.Collections.Generic.List<Msg.Protocol.SqlMsg> _sqls = new global::System.Collections.Generic.List<Msg.Protocol.SqlMsg>();
    [global::ProtoBuf.ProtoMember(3, Name=@"sqls", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public global::System.Collections.Generic.List<Msg.Protocol.SqlMsg> sqls
    {
      get { return _sqls; }
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
    [global::ProtoBuf.ProtoContract(Name=@"GameToDBMsgId")]
    public enum GameToDBMsgId
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"eGameToDBMsgIdMin", Value=2000)]
      eGameToDBMsgIdMin = 2000,
            
      [global::ProtoBuf.ProtoEnum(Name=@"ePostSql", Value=2001)]
      ePostSql = 2001,
            
      [global::ProtoBuf.ProtoEnum(Name=@"eGameToDBMsgIdMax", Value=3000)]
      eGameToDBMsgIdMax = 3000
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"SqlType")]
    public enum SqlType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"NormalSql", Value=0)]
      NormalSql = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"PlayerSql", Value=1)]
      PlayerSql = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"GuidSql", Value=2)]
      GuidSql = 2
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"SqlDBType")]
    public enum SqlDBType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"GameDB", Value=0)]
      GameDB = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"LogDB", Value=1)]
      LogDB = 1
    }
  
}