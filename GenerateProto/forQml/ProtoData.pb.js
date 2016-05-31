.pragma library
.import Protobuf 1.0 as Protobuf
.import 'StaticData.pb.js' as Q__StaticData__
.import 'StatusData.pb.js' as Q__StatusData__
.import 'ExData.pb.js' as Q__ExData__
'use strict';

var _file = {
  get descriptor() {
    if(!this._desc && Q__StaticData__._file.descriptor && Q__StatusData__._file.descriptor && Q__ExData__._file.descriptor) {
      this._desc = Protobuf.DescriptorPool.addFileDescriptor('Cg9Qcm90b0RhdGEucHJvdG8SCmZhbnRhc3liaXQaEFN0YXRpY0RhdGEucHJvdG8aEFN0YXR1c0RhdGEucHJvdG8aDEV4RGF0YS5wcm90byJHCg1NeUZhbnRhc3lOYW1lEgwKBG5hbWUYASABKAkSKAoGc3RhdHVzGAIgASgOMhguZmFudGFzeWJpdC5NeU5hbWVTdGF0dXMiKwoNRmFudGFzeVBsYXllchIMCgRuYW1lGAogASgJEgwKBGJpdHMYFCABKAQieAoGU2VjcmV0EhMKC3ByaXZhdGVfa2V5GAEgAigJEjAKDW15ZmFudGFzeW5hbWUYAiABKAsyGS5mYW50YXN5Yml0Lk15RmFudGFzeU5hbWUSJwoIbmFtZXRyYW4YCiABKAsyFS5mYW50YXN5Yml0Lk5hbWVUcmFucyJICgdTZWNyZXQyEhMKC3ByaXZhdGVfa2V5GAEgASgJEhQKDGZhbnRhc3lfbmFtZRgUIAEoCRISCgpwdWJsaWNfa2V5GB4gASgJIl4KB1NlY3JldDMSEwoLcHJpdmF0ZV9rZXkYASABKAkSFAoMbW5lbW9uaWNfa2V5GAMgASgJEhQKDGZhbnRhc3lfbmFtZRgUIAEoCRISCgpwdWJsaWNfa2V5GB4gASgJIqUBCgdPdXREYXRhEiYKBHR5cGUYASACKA4yGC5mYW50YXN5Yml0Lk91dERhdGEuVHlwZRIwCg1teWZhbnRhc3luYW1lGAIgASgLMhkuZmFudGFzeWJpdC5NeUZhbnRhc3lOYW1lIjYKBFR5cGUSEQoNTVlGQU5UQVNZTkFNRRABEgwKCFNOQVBTSE9UEAISDQoJSEVBUlRCRUFUEAMqCAhkEICAgIACIoACCgZJbkRhdGESJQoEdHlwZRgBIAIoDjIXLmZhbnRhc3liaXQuSW5EYXRhLlR5cGUSDAoEZGF0YRgCIAEoCRINCgVkYXRhMhgDIAEoCRILCgNudW0YBCABKAUSLgoKZGF0YV90cmFucxgFIAEoCzIaLmZhbnRhc3liaXQuRGF0YVRyYW5zaXRpb24iawoEVHlwZRIMCghNSU5FTkFNRRABEggKBFFVSVQQAhINCglIRUFSVEJFQVQQAxILCgdDT05ORUNUEAQSDgoKTUFLRV9CTE9DSxAFEgsKB05FV05BTUUQBhIICgRQUk9KEAcSCAoEREFUQRAIKggIZBCAgICAAiJ0CglOYW1lUHJvb2YSKAoEdHlwZRgBIAEoDjIaLmZhbnRhc3liaXQuTmFtZVByb29mLlR5cGUiMgoEVHlwZRIHCgNQT1cQARIJCgVUV0VFVBACEgoKBk9SQUNMRRADEgoKBk1BU1RFUhAEKgkIyAEQgICAgAIiYAoKVHdlZXRQcm9vZhINCgV0d2VldBgKIAEoCTJDCgt0d2VldF9wcm9vZhIVLmZhbnRhc3liaXQuTmFtZVByb29mGMkBIAEoCzIWLmZhbnRhc3liaXQuVHdlZXRQcm9vZiK9AQoLTWFzdGVyUHJvb2YSDgoGc2Vhc29uGAogASgJEgwKBHdlZWsYFCABKAUSEQoJdGltZXN0YW1wGBUgASgFEjYKD25ld19vcmFjbGVfbmFtZRhCIAEoCzIdLmZhbnRhc3liaXQuU2lnbmVkVHJhbnNhY3Rpb24yRQoMbWFzdGVyX3Byb29mEhUuZmFudGFzeWJpdC5OYW1lUHJvb2YYkQMgASgLMhcuZmFudGFzeWJpdC5NYXN0ZXJQcm9vZiJNCgtUcmFuc2FjdGlvbhIPCgd2ZXJzaW9uGAEgASgFEiMKBHR5cGUYAiABKA4yFS5mYW50YXN5Yml0LlRyYW5zVHlwZSoICGQQgICAgAIioAEKCU5hbWVUcmFucxIUCgxmYW50YXN5X25hbWUYCiABKAkSEgoKcHVibGljX2tleRgUIAEoDBIkCgVwcm9vZhgeIAEoCzIVLmZhbnRhc3liaXQuTmFtZVByb29mMkMKCm5hbWVfdHJhbnMSFy5mYW50YXN5Yml0LlRyYW5zYWN0aW9uGMgBIAEoCzIVLmZhbnRhc3liaXQuTmFtZVRyYW5zImoKEVNpZ25lZFRyYW5zYWN0aW9uEiYKBXRyYW5zGAogASgLMhcuZmFudGFzeWJpdC5UcmFuc2FjdGlvbhIKCgJpZBgUIAEoDBILCgNzaWcYHiABKAwSFAoMZmFudGFzeV9uYW1lGCggASgJIvgBCgtCbG9ja0hlYWRlchIPCgd2ZXJzaW9uGAEgASgFEgsKA251bRgKIAEoBRIPCgdwcmV2X2lkGBQgASgMEhEKCXRpbWVzdGFtcBgVIAEoBRIUCgxnZW5lcmF0b3JfcGsYHiABKAwSFgoOZ2VuZXJhdGluZ19zaWcYKCABKAwSEgoKYmFzZXRhcmdldBgyIAEoBRIvCglibG9ja3R5cGUYPCABKA4yHC5mYW50YXN5Yml0LkJsb2NrSGVhZGVyLlR5cGUSFgoOdHJhbnNhY3Rpb25faWQYRiABKAwiHAoEVHlwZRIKCgZOT1JNQUwQARIICgREQVRBEAIiRwoRU2lnbmVkQmxvY2tIZWFkZXISJQoEaGVhZBgKIAEoCzIXLmZhbnRhc3liaXQuQmxvY2tIZWFkZXISCwoDc2lnGB4gASgJIoEBCgVCbG9jaxIxCgpzaWduZWRoZWFkGAogASgLMh0uZmFudGFzeWJpdC5TaWduZWRCbG9ja0hlYWRlchI6ChNzaWduZWRfdHJhbnNhY3Rpb25zGBQgAygLMh0uZmFudGFzeWJpdC5TaWduZWRUcmFuc2FjdGlvbioJCOgHEICAgIACIq8BCgtOb2RlUmVxdWVzdBIqCgR0eXBlGAogASgOMhwuZmFudGFzeWJpdC5Ob2RlUmVxdWVzdC5UeXBlEgwKBG15aXAYFCABKAkSCwoDbnVtGB4gASgFEg4KBm15aG9zdBgoIAEoCSJJCgRUeXBlEg0KCUhBTkRTSEFLRRABEhEKDUJMT0NLX1JFUVVFU1QQAhIRCg1ISUdIVF9SRVFVRVNUEAMSDAoITkFUX1RFU1QQBCInCglOb2RlUmVwbHkSDQoFaGlnaHQYCiABKAUSCwoDaXBzGBQgAygJImUKE0ZhbnRhc3lQbGF5ZXJQb2ludHMSDgoGc2Vhc29uGAogASgNEgwKBHdlZWsYFCABKA0SEAoIcGxheWVyaWQYHiABKAkSDgoGcG9pbnRzGCggASgFEg4KBnJlc3VsdBgyIAEoAiKcAQoPUHJvamVjdGlvblRyYW5zEg4KBnNlYXNvbhgKIAEoDRIMCgR3ZWVrGBQgASgNEhAKCHBsYXllcmlkGB4gASgJEg4KBnBvaW50cxgoIAEoBTJJCgpwcm9qX3RyYW5zEhcuZmFudGFzeWJpdC5UcmFuc2FjdGlvbhjJASABKAsyGy5mYW50YXN5Yml0LlByb2plY3Rpb25UcmFucyIwCgxQbGF5ZXJQb2ludHMSEAoIcGxheWVyaWQYHiABKAkSDgoGcG9pbnRzGCggASgFIrsBChRQcm9qZWN0aW9uVHJhbnNCbG9jaxIOCgZzZWFzb24YCiABKA0SDAoEd2VlaxgUIAEoDRIvCg1wbGF5ZXJfcG9pbnRzGB4gAygLMhguZmFudGFzeWJpdC5QbGF5ZXJQb2ludHMyVAoQcHJval90cmFuc19ibG9jaxIXLmZhbnRhc3liaXQuVHJhbnNhY3Rpb24Y0wEgASgLMiAuZmFudGFzeWJpdC5Qcm9qZWN0aW9uVHJhbnNCbG9jayKLAQoERGF0YRIPCgd2ZXJzaW9uGAogASgFEiMKBHR5cGUYFCABKA4yFS5mYW50YXN5Yml0LkRhdGEuVHlwZSJDCgRUeXBlEgwKCFNDSEVEVUxFEAASCgoGUExBWUVSEAESCAoER0FNRRACEgoKBlJFU1VMVBADEgsKB01FU1NBR0UQBCoICGQQgICAgAIi9QEKClBsYXllckRhdGESEAoIcGxheWVyaWQYCiABKAkSKwoLcGxheWVyX2Jhc2UYFCABKAsyFi5mYW50YXN5Yml0LlBsYXllckJhc2USLwoNcGxheWVyX3N0YXR1cxgeIAEoCzIYLmZhbnRhc3liaXQuUGxheWVyU3RhdHVzEjgKEnBsYXllcl9nYW1lX3N0YXR1cxgoIAEoDjIcLmZhbnRhc3liaXQuUGxheWVyR2FtZVN0YXR1czI9CgtwbGF5ZXJfZGF0YRIQLmZhbnRhc3liaXQuRGF0YRhlIAEoCzIWLmZhbnRhc3liaXQuUGxheWVyRGF0YSJ9CghHYW1lRGF0YRIOCgZnYW1laWQYCiABKAkSJgoGc3RhdHVzGBQgASgLMhYuZmFudGFzeWJpdC5HYW1lU3RhdHVzMjkKCWdhbWVfZGF0YRIQLmZhbnRhc3liaXQuRGF0YRhvIAEoCzIULmZhbnRhc3liaXQuR2FtZURhdGEieQoKUmVzdWx0RGF0YRIrCgtnYW1lX3Jlc3VsdBgKIAEoCzIWLmZhbnRhc3liaXQuR2FtZVJlc3VsdDI+CgtyZXN1bHRfZGF0YRIQLmZhbnRhc3liaXQuRGF0YRjKASABKAsyFi5mYW50YXN5Yml0LlJlc3VsdERhdGEijAEKDFNjaGVkdWxlRGF0YRIMCgR3ZWVrGAEgASgFEioKBndlZWtseRgKIAEoCzIaLmZhbnRhc3liaXQuV2Vla2x5U2NoZWR1bGUyQgoNc2NoZWR1bGVfZGF0YRIQLmZhbnRhc3liaXQuRGF0YRiuAiABKAsyGC5mYW50YXN5Yml0LlNjaGVkdWxlRGF0YSLiAQoORGF0YVRyYW5zaXRpb24SIAoEdHlwZRgBIAEoDjISLmZhbnRhc3liaXQuVHJUeXBlEg4KBnNlYXNvbhgKIAEoDRIMCgR3ZWVrGBQgASgNEiYKCGdhbWVkYXRhGB4gAygLMhQuZmFudGFzeWJpdC5HYW1lRGF0YRIeCgRkYXRhGCggAygLMhAuZmFudGFzeWJpdC5EYXRhMkgKCmRhdGFfdHJhbnMSFy5mYW50YXN5Yml0LlRyYW5zYWN0aW9uGMsBIAEoCzIaLmZhbnRhc3liaXQuRGF0YVRyYW5zaXRpb24idwoJVGVhbVN0YXRlEioKBXN0YXRlGAEgASgOMhsuZmFudGFzeWJpdC5UZWFtU3RhdGUuU3RhdGUSDAoEd2VlaxgKIAEoDRIOCgZ0ZWFtaWQYFCABKAkiIAoFU3RhdGUSCwoHUFJFR0FNRRABEgoKBklOR0FNRRACIr0CCglEZWx0YURhdGESKAoEdHlwZRgBIAEoDjIaLmZhbnRhc3liaXQuRGVsdGFEYXRhLlR5cGUSMAoNbXlmYW50YXN5bmFtZRgCIAMoCzIZLmZhbnRhc3liaXQuTXlGYW50YXN5TmFtZRIsCgtnbG9iYWxzdGF0ZRgKIAEoCzIXLmZhbnRhc3liaXQuR2xvYmFsU3RhdGUSKQoKdGVhbXN0YXRlcxgUIAMoCzIVLmZhbnRhc3liaXQuVGVhbVN0YXRlEh8KBWRhdGFzGB4gAygLMhAuZmFudGFzeWJpdC5EYXRhEioKB3BsYXllcnMYKCADKAsyGS5mYW50YXN5Yml0LkZhbnRhc3lQbGF5ZXIiIwoEVHlwZRIMCghTTkFQU0hPVBACEg0KCUhFQVJUQkVBVBADKgkI6AcQgICAgAIidAoLTWVzc2FnZURhdGESCwoDbXNnGAogASgJEgoKAmd0GBQgASgFEgoKAmx0GB4gASgFMkAKDG1lc3NhZ2VfZGF0YRIQLmZhbnRhc3liaXQuRGF0YRiUAyABKAsyFy5mYW50YXN5Yml0Lk1lc3NhZ2VEYXRhIsEBCgxTdGFtcGVkVHJhbnMSEQoJdGltZXN0YW1wGAogASgEEg4KBnNlcW51bRgUIAEoBRIPCgdwcmV2c2VxGBUgASgFEjIKC3NpZ25lZF9vcmlnGB4gASgLMh0uZmFudGFzeWJpdC5TaWduZWRUcmFuc2FjdGlvbjJJCg1zdGFtcGVkX3RyYW5zEhcuZmFudGFzeWJpdC5UcmFuc2FjdGlvbhisAiABKAsyGC5mYW50YXN5Yml0LlN0YW1wZWRUcmFucyKLAgoNRXhjaGFuZ2VPcmRlchIsCgR0eXBlGAogASgOMh4uZmFudGFzeWJpdC5FeGNoYW5nZU9yZGVyLlR5cGUSEAoIcGxheWVyaWQYKCABKAkSIwoEY29yZRgyIAEoCzIVLmZhbnRhc3liaXQuT3JkZXJDb3JlEhMKC2NhbmNlbF9vcmVmGGQgASgFIigKBFR5cGUSBwoDTkVXEAESCgoGQ0FOQ0VMEAISCwoHUkVQTEFDRRADKgkI9AMQgICAgAIySwoOZXhjaGFuZ2Vfb3JkZXISFy5mYW50YXN5Yml0LlRyYW5zYWN0aW9uGLYCIAEoCzIZLmZhbnRhc3liaXQuRXhjaGFuZ2VPcmRlciJxCgtPcmRlclVuaXF1ZRISCgpwdWJsaWNfa2V5GAogASgJEg4KBnNlYXNvbhgUIAEoDRIMCgR3ZWVrGB4gASgNEg8KB2J1eXNpZGUYKCABKAgSEAoIcGxheWVyaWQYMiABKAkSDQoFcHJpY2UYRiABKAUiTwoKT3JkZXJEZWV0cxILCgNvaWQYCiABKAwSJgoFb3JkZXIYFCABKAsyFy5mYW50YXN5Yml0Lk9yZGVyVW5pcXVlEgwKBHNpemUYHiABKAUilwEKDEluc2lkZVVuaXF1ZRIQCghwbGF5ZXJpZBgyIAEoCRISCgpwdWJsaWNfa2V5GAogASgJEg8KB2J1eXNpZGUYKCABKAgSDQoFcHJpY2UYRiABKAUSCwoDb2lkGAsgASgMEiYKBW9yZGVyGBQgASgLMhcuZmFudGFzeWJpdC5PcmRlclVuaXF1ZRIMCgRzaXplGB4gASgFIs4BCg5UaW1lVHJhbnNpdGlvbhItCgR0eXBlGAEgASgOMh8uZmFudGFzeWJpdC5UaW1lVHJhbnNpdGlvbi5UeXBlEg4KBnNlYXNvbhgKIAEoDRIMCgR3ZWVrGBQgASgNIiUKBFR5cGUSDQoJVFJBREVPUEVOEAISDgoKVFJBREVDTE9TRRADMkgKCnRpbWVfdHJhbnMSFy5mYW50YXN5Yml0LlRyYW5zYWN0aW9uGK8CIAEoCzIaLmZhbnRhc3liaXQuVGltZVRyYW5zaXRpb24qWQoMTXlOYW1lU3RhdHVzEggKBG5vbmUQARILCgdub3RhdmlsEAISDQoJcmVxdWVzdGVkEAUSFAoQdHJhbnNhY3Rpb25fc2VudBAPEg0KCWNvbmZpcm1lZBAUKpsBCglUcmFuc1R5cGUSCAoETkFNRRAAEg4KClBST0pFQ1RJT04QARIKCgZSRVNVTFQQAhIICgREQVRBEAMSFAoQUFJPSkVDVElPTl9CTE9DSxAEEg8KC01BU1RFUl9OQU1FEAUSCAoEVElNRRAGEgsKB1NUQU1QRUQQBxIMCghFWENIQU5HRRAIEhIKDkVYQ0hBTkdFX0JMT0NLEAkqjAEKBlRyVHlwZRIPCgtTRUFTT05TVEFSVBACEg0KCVNFQVNPTkVORBADEg0KCUhFQVJUQkVBVBAFEg0KCUdBTUVTVEFSVBAGEgwKCFdFRUtPVkVSEAcSFQoRVFJBREVTRVNTSU9OU1RBUlQQCBIfChtUUkFERVNFU1NJT05DTE9TRUFORFBSRU9QRU4QCQ==', 5839);
    }
    if (!this._desc)
      console.warn('Failed to initialize: ProtoData.proto');
    return this._desc;
  },
};


var MyNameStatus = {
  none: 1,
  notavil: 2,
  requested: 5,
  transaction_sent: 15,
  confirmed: 20,

  toString: function(value) {
    switch(value) {
      case 1: return 'none';
      case 2: return 'notavil';
      case 5: return 'requested';
      case 15: return 'transaction_sent';
      case 20: return 'confirmed';
    }
  },
};

var TransType = {
  NAME: 0,
  PROJECTION: 1,
  RESULT: 2,
  DATA: 3,
  PROJECTION_BLOCK: 4,
  MASTER_NAME: 5,
  TIME: 6,
  STAMPED: 7,
  EXCHANGE: 8,
  EXCHANGE_BLOCK: 9,

  toString: function(value) {
    switch(value) {
      case 0: return 'NAME';
      case 1: return 'PROJECTION';
      case 2: return 'RESULT';
      case 3: return 'DATA';
      case 4: return 'PROJECTION_BLOCK';
      case 5: return 'MASTER_NAME';
      case 6: return 'TIME';
      case 7: return 'STAMPED';
      case 8: return 'EXCHANGE';
      case 9: return 'EXCHANGE_BLOCK';
    }
  },
};

var TrType = {
  SEASONSTART: 2,
  SEASONEND: 3,
  HEARTBEAT: 5,
  GAMESTART: 6,
  WEEKOVER: 7,
  TRADESESSIONSTART: 8,
  TRADESESSIONCLOSEANDPREOPEN: 9,

  toString: function(value) {
    switch(value) {
      case 2: return 'SEASONSTART';
      case 3: return 'SEASONEND';
      case 5: return 'HEARTBEAT';
      case 6: return 'GAMESTART';
      case 7: return 'WEEKOVER';
      case 8: return 'TRADESESSIONSTART';
      case 9: return 'TRADESESSIONCLOSEANDPREOPEN';
    }
  },
};
var MyFantasyName = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof MyFantasyName) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(0));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setName(rawArray[0][0]);
      this.setStatus(rawArray[0][1]);
    }
  };

  type.prototype.getName = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setName = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearName = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'name', {
    get: type.prototype.getName,
    set: type.prototype.setName,
  });

  type.prototype.getStatus = function() {
    return typeof this._fields[1] == 'undefined' ? MyNameStatus.none : this._fields[1];
  }
  type.prototype.setStatus = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearStatus = function() {
    this._fields[1] = MyNameStatus.none;
  };
  Object.defineProperty(type.prototype, 'status', {
    get: type.prototype.getStatus,
    set: type.prototype.setStatus,
  });

  return type;
})();

var FantasyPlayer = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyPlayer) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(1));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setName(rawArray[0][0]);
      this.setBits(rawArray[0][1]);
    }
  };

  type.prototype.getName = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setName = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearName = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'name', {
    get: type.prototype.getName,
    set: type.prototype.setName,
  });

  type.prototype.getBits = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setBits = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearBits = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'bits', {
    get: type.prototype.getBits,
    set: type.prototype.setBits,
  });

  return type;
})();

var Secret = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Secret) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(2));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPrivateKey(rawArray[0][0]);
      if (typeof this.myfantasyname == 'undefined') {
        this.setMyfantasyname({});
      }
      this._myfantasyname._mergeFromRawArray(rawArray[0][1]);
      if (typeof this.nametran == 'undefined') {
        this.setNametran({});
      }
      this._nametran._mergeFromRawArray(rawArray[0][2]);
    }
  };

  type.prototype.getPrivateKey = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPrivateKey = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPrivateKey = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'privateKey', {
    get: type.prototype.getPrivateKey,
    set: type.prototype.setPrivateKey,
  });

  type.prototype.getMyfantasyname = function() {
    return this._myfantasyname;
  }
  type.prototype.setMyfantasyname = function(value) {
    var msg = this._maybeConvertToMessage(MyFantasyName, value);
    this._myfantasyname = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearMyfantasyname = function() {
    this._fields[1] = undefined;
    this._myfantasyname = undefined;
  };
  Object.defineProperty(type.prototype, 'myfantasyname', {
    get: type.prototype.getMyfantasyname,
    set: type.prototype.setMyfantasyname,
  });

  type.prototype.getNametran = function() {
    return this._nametran;
  }
  type.prototype.setNametran = function(value) {
    var msg = this._maybeConvertToMessage(NameTrans, value);
    this._nametran = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearNametran = function() {
    this._fields[2] = undefined;
    this._nametran = undefined;
  };
  Object.defineProperty(type.prototype, 'nametran', {
    get: type.prototype.getNametran,
    set: type.prototype.setNametran,
  });

  return type;
})();

var Secret2 = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Secret2) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(3));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPrivateKey(rawArray[0][0]);
      this.setFantasyName(rawArray[0][1]);
      this.setPublicKey(rawArray[0][2]);
    }
  };

  type.prototype.getPrivateKey = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPrivateKey = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPrivateKey = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'privateKey', {
    get: type.prototype.getPrivateKey,
    set: type.prototype.setPrivateKey,
  });

  type.prototype.getFantasyName = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setFantasyName = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearFantasyName = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'fantasyName', {
    get: type.prototype.getFantasyName,
    set: type.prototype.setFantasyName,
  });

  type.prototype.getPublicKey = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setPublicKey = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPublicKey = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'publicKey', {
    get: type.prototype.getPublicKey,
    set: type.prototype.setPublicKey,
  });

  return type;
})();

var Secret3 = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Secret3) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(4));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPrivateKey(rawArray[0][0]);
      this.setMnemonicKey(rawArray[0][1]);
      this.setFantasyName(rawArray[0][2]);
      this.setPublicKey(rawArray[0][3]);
    }
  };

  type.prototype.getPrivateKey = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPrivateKey = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPrivateKey = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'privateKey', {
    get: type.prototype.getPrivateKey,
    set: type.prototype.setPrivateKey,
  });

  type.prototype.getMnemonicKey = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setMnemonicKey = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearMnemonicKey = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'mnemonicKey', {
    get: type.prototype.getMnemonicKey,
    set: type.prototype.setMnemonicKey,
  });

  type.prototype.getFantasyName = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setFantasyName = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearFantasyName = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'fantasyName', {
    get: type.prototype.getFantasyName,
    set: type.prototype.setFantasyName,
  });

  type.prototype.getPublicKey = function() {
    return typeof this._fields[3] == 'undefined' ? '' : this._fields[3];
  }
  type.prototype.setPublicKey = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPublicKey = function() {
    this._fields[3] = '';
  };
  Object.defineProperty(type.prototype, 'publicKey', {
    get: type.prototype.getPublicKey,
    set: type.prototype.setPublicKey,
  });

  return type;
})();

var OutData = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof OutData) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    MYFANTASYNAME: 1,
    SNAPSHOT: 2,
    HEARTBEAT: 3,
  
    toString: function(value) {
      switch(value) {
        case 1: return 'MYFANTASYNAME';
        case 2: return 'SNAPSHOT';
        case 3: return 'HEARTBEAT';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(5));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
      if (typeof this.myfantasyname == 'undefined') {
        this.setMyfantasyname({});
      }
      this._myfantasyname._mergeFromRawArray(rawArray[0][1]);
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? OutData.Type.MYFANTASYNAME : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = OutData.Type.MYFANTASYNAME;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  type.prototype.getMyfantasyname = function() {
    return this._myfantasyname;
  }
  type.prototype.setMyfantasyname = function(value) {
    var msg = this._maybeConvertToMessage(MyFantasyName, value);
    this._myfantasyname = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearMyfantasyname = function() {
    this._fields[1] = undefined;
    this._myfantasyname = undefined;
  };
  Object.defineProperty(type.prototype, 'myfantasyname', {
    get: type.prototype.getMyfantasyname,
    set: type.prototype.setMyfantasyname,
  });

  return type;
})();

var InData = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof InData) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    MINENAME: 1,
    QUIT: 2,
    HEARTBEAT: 3,
    CONNECT: 4,
    MAKE_BLOCK: 5,
    NEWNAME: 6,
    PROJ: 7,
    DATA: 8,
  
    toString: function(value) {
      switch(value) {
        case 1: return 'MINENAME';
        case 2: return 'QUIT';
        case 3: return 'HEARTBEAT';
        case 4: return 'CONNECT';
        case 5: return 'MAKE_BLOCK';
        case 6: return 'NEWNAME';
        case 7: return 'PROJ';
        case 8: return 'DATA';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(6));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
      this.setData(rawArray[0][1]);
      this.setData2(rawArray[0][2]);
      this.setNum(rawArray[0][3]);
      if (typeof this.dataTrans == 'undefined') {
        this.setDataTrans({});
      }
      this._dataTrans._mergeFromRawArray(rawArray[0][4]);
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? InData.Type.MINENAME : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = InData.Type.MINENAME;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  type.prototype.getData = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setData = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearData = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'data', {
    get: type.prototype.getData,
    set: type.prototype.setData,
  });

  type.prototype.getData2 = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setData2 = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearData2 = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'data2', {
    get: type.prototype.getData2,
    set: type.prototype.setData2,
  });

  type.prototype.getNum = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setNum = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearNum = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'num', {
    get: type.prototype.getNum,
    set: type.prototype.setNum,
  });

  type.prototype.getDataTrans = function() {
    return this._dataTrans;
  }
  type.prototype.setDataTrans = function(value) {
    var msg = this._maybeConvertToMessage(DataTransition, value);
    this._dataTrans = msg;
    this._fields[4] = msg._raw;
  };
  type.prototype.clearDataTrans = function() {
    this._fields[4] = undefined;
    this._dataTrans = undefined;
  };
  Object.defineProperty(type.prototype, 'dataTrans', {
    get: type.prototype.getDataTrans,
    set: type.prototype.setDataTrans,
  });

  return type;
})();

var NameProof = (function() {
  var type = function(values) {
    this._fields = new Array(1);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof NameProof) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    POW: 1,
    TWEET: 2,
    ORACLE: 3,
    MASTER: 4,
  
    toString: function(value) {
      switch(value) {
        case 1: return 'POW';
        case 2: return 'TWEET';
        case 3: return 'ORACLE';
        case 4: return 'MASTER';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(7));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? NameProof.Type.POW : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = NameProof.Type.POW;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  return type;
})();

var TweetProof = (function() {
  var type = function(values) {
    this._fields = new Array(1);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TweetProof) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(8));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setTweet(rawArray[0][0]);
    }
  };

  type.prototype.getTweet = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setTweet = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTweet = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'tweet', {
    get: type.prototype.getTweet,
    set: type.prototype.setTweet,
  });

  return type;
})();

var MasterProof = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof MasterProof) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(9));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSeason(rawArray[0][0]);
      this.setWeek(rawArray[0][1]);
      this.setTimestamp(rawArray[0][2]);
      if (typeof this.newOracleName == 'undefined') {
        this.setNewOracleName({});
      }
      this._newOracleName._mergeFromRawArray(rawArray[0][3]);
    }
  };

  type.prototype.getSeason = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setSeason = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setWeek = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getTimestamp = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setTimestamp = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearTimestamp = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'timestamp', {
    get: type.prototype.getTimestamp,
    set: type.prototype.setTimestamp,
  });

  type.prototype.getNewOracleName = function() {
    return this._newOracleName;
  }
  type.prototype.setNewOracleName = function(value) {
    var msg = this._maybeConvertToMessage(SignedTransaction, value);
    this._newOracleName = msg;
    this._fields[3] = msg._raw;
  };
  type.prototype.clearNewOracleName = function() {
    this._fields[3] = undefined;
    this._newOracleName = undefined;
  };
  Object.defineProperty(type.prototype, 'newOracleName', {
    get: type.prototype.getNewOracleName,
    set: type.prototype.setNewOracleName,
  });

  return type;
})();

var Transaction = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Transaction) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(10));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setVersion(rawArray[0][0]);
      this.setType(rawArray[0][1]);
    }
  };

  type.prototype.getVersion = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setVersion = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearVersion = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'version', {
    get: type.prototype.getVersion,
    set: type.prototype.setVersion,
  });

  type.prototype.getType = function() {
    return typeof this._fields[1] == 'undefined' ? TransType.NAME : this._fields[1];
  }
  type.prototype.setType = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearType = function() {
    this._fields[1] = TransType.NAME;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  return type;
})();

var NameTrans = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof NameTrans) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(11));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setFantasyName(rawArray[0][0]);
      this.setPublicKey(rawArray[0][1]);
      if (typeof this.proof == 'undefined') {
        this.setProof({});
      }
      this._proof._mergeFromRawArray(rawArray[0][2]);
    }
  };

  type.prototype.getFantasyName = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setFantasyName = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearFantasyName = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'fantasyName', {
    get: type.prototype.getFantasyName,
    set: type.prototype.setFantasyName,
  });

  type.prototype.getPublicKey = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setPublicKey = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPublicKey = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'publicKey', {
    get: type.prototype.getPublicKey,
    set: type.prototype.setPublicKey,
  });

  type.prototype.getProof = function() {
    return this._proof;
  }
  type.prototype.setProof = function(value) {
    var msg = this._maybeConvertToMessage(NameProof, value);
    this._proof = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearProof = function() {
    this._fields[2] = undefined;
    this._proof = undefined;
  };
  Object.defineProperty(type.prototype, 'proof', {
    get: type.prototype.getProof,
    set: type.prototype.setProof,
  });

  return type;
})();

var SignedTransaction = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof SignedTransaction) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(12));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      if (typeof this.trans == 'undefined') {
        this.setTrans({});
      }
      this._trans._mergeFromRawArray(rawArray[0][0]);
      this.setId(rawArray[0][1]);
      this.setSig(rawArray[0][2]);
      this.setFantasyName(rawArray[0][3]);
    }
  };

  type.prototype.getTrans = function() {
    return this._trans;
  }
  type.prototype.setTrans = function(value) {
    var msg = this._maybeConvertToMessage(Transaction, value);
    this._trans = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearTrans = function() {
    this._fields[0] = undefined;
    this._trans = undefined;
  };
  Object.defineProperty(type.prototype, 'trans', {
    get: type.prototype.getTrans,
    set: type.prototype.setTrans,
  });

  type.prototype.getId = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setId = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearId = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'id', {
    get: type.prototype.getId,
    set: type.prototype.setId,
  });

  type.prototype.getSig = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setSig = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearSig = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'sig', {
    get: type.prototype.getSig,
    set: type.prototype.setSig,
  });

  type.prototype.getFantasyName = function() {
    return typeof this._fields[3] == 'undefined' ? '' : this._fields[3];
  }
  type.prototype.setFantasyName = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearFantasyName = function() {
    this._fields[3] = '';
  };
  Object.defineProperty(type.prototype, 'fantasyName', {
    get: type.prototype.getFantasyName,
    set: type.prototype.setFantasyName,
  });

  return type;
})();

var BlockHeader = (function() {
  var type = function(values) {
    this._fields = new Array(9);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof BlockHeader) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    NORMAL: 1,
    DATA: 2,
  
    toString: function(value) {
      switch(value) {
        case 1: return 'NORMAL';
        case 2: return 'DATA';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(13));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setVersion(rawArray[0][0]);
      this.setNum(rawArray[0][1]);
      this.setPrevId(rawArray[0][2]);
      this.setTimestamp(rawArray[0][3]);
      this.setGeneratorPk(rawArray[0][4]);
      this.setGeneratingSig(rawArray[0][5]);
      this.setBasetarget(rawArray[0][6]);
      this.setBlocktype(rawArray[0][7]);
      this.setTransactionId(rawArray[0][8]);
    }
  };

  type.prototype.getVersion = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setVersion = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearVersion = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'version', {
    get: type.prototype.getVersion,
    set: type.prototype.setVersion,
  });

  type.prototype.getNum = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setNum = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearNum = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'num', {
    get: type.prototype.getNum,
    set: type.prototype.setNum,
  });

  type.prototype.getPrevId = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setPrevId = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrevId = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prevId', {
    get: type.prototype.getPrevId,
    set: type.prototype.setPrevId,
  });

  type.prototype.getTimestamp = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setTimestamp = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearTimestamp = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'timestamp', {
    get: type.prototype.getTimestamp,
    set: type.prototype.setTimestamp,
  });

  type.prototype.getGeneratorPk = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setGeneratorPk = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearGeneratorPk = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'generatorPk', {
    get: type.prototype.getGeneratorPk,
    set: type.prototype.setGeneratorPk,
  });

  type.prototype.getGeneratingSig = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setGeneratingSig = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearGeneratingSig = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'generatingSig', {
    get: type.prototype.getGeneratingSig,
    set: type.prototype.setGeneratingSig,
  });

  type.prototype.getBasetarget = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setBasetarget = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearBasetarget = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'basetarget', {
    get: type.prototype.getBasetarget,
    set: type.prototype.setBasetarget,
  });

  type.prototype.getBlocktype = function() {
    return typeof this._fields[7] == 'undefined' ? BlockHeader.Type.NORMAL : this._fields[7];
  }
  type.prototype.setBlocktype = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearBlocktype = function() {
    this._fields[7] = BlockHeader.Type.NORMAL;
  };
  Object.defineProperty(type.prototype, 'blocktype', {
    get: type.prototype.getBlocktype,
    set: type.prototype.setBlocktype,
  });

  type.prototype.getTransactionId = function() {
    return typeof this._fields[8] == 'undefined' ?  new ArrayBuffer() : this._fields[8];
  }
  type.prototype.setTransactionId = function(value) {
      this._fields[8] = value;
  };
  type.prototype.clearTransactionId = function() {
    this._fields[8] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'transactionId', {
    get: type.prototype.getTransactionId,
    set: type.prototype.setTransactionId,
  });

  return type;
})();

var SignedBlockHeader = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof SignedBlockHeader) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(14));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      if (typeof this.head == 'undefined') {
        this.setHead({});
      }
      this._head._mergeFromRawArray(rawArray[0][0]);
      this.setSig(rawArray[0][1]);
    }
  };

  type.prototype.getHead = function() {
    return this._head;
  }
  type.prototype.setHead = function(value) {
    var msg = this._maybeConvertToMessage(BlockHeader, value);
    this._head = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearHead = function() {
    this._fields[0] = undefined;
    this._head = undefined;
  };
  Object.defineProperty(type.prototype, 'head', {
    get: type.prototype.getHead,
    set: type.prototype.setHead,
  });

  type.prototype.getSig = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setSig = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSig = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'sig', {
    get: type.prototype.getSig,
    set: type.prototype.setSig,
  });

  return type;
})();

var Block = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._signedTransactions = new Array();
  this._fields[1] = new Array();
  if (values && values.signedTransactions && values.signedTransactions instanceof Array) {
    this.signedTransactions(values.signedTransactions);
  }
    if (values instanceof Block) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(15));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      if (typeof this.signedhead == 'undefined') {
        this.setSignedhead({});
      }
      this._signedhead._mergeFromRawArray(rawArray[0][0]);
      field = rawArray[0][1];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._signedTransactions[i] == 'undefined') {
            var msg = new SignedTransaction();
            this._signedTransactions[i] = msg;
            this._fields[1][i] = msg._raw;
          }
          this._signedTransactions[i]._mergeFromRawArray(field[i]);
        }
      }
    }
  };

  type.prototype.getSignedhead = function() {
    return this._signedhead;
  }
  type.prototype.setSignedhead = function(value) {
    var msg = this._maybeConvertToMessage(SignedBlockHeader, value);
    this._signedhead = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearSignedhead = function() {
    this._fields[0] = undefined;
    this._signedhead = undefined;
  };
  Object.defineProperty(type.prototype, 'signedhead', {
    get: type.prototype.getSignedhead,
    set: type.prototype.setSignedhead,
  });

    // Replacement setter
  type.prototype.setSignedTransactions = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[1].length = values.length;
     this._signedTransactions.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new SignedTransaction(values[i]);
       this._signedTransactions[i] = msg;
       this._fields[1][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setSignedTransactionsAt = function(index, value) {
    var msg = this._maybeConvertToMessage(SignedTransaction, value);
    this._signedTransactions[index] = msg;
    this._fields[1][index] = msg._raw;
  };
  // Getter
  type.prototype.getSignedTransactionsAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._signedTransactions[index];
  };
  type.prototype.getSignedTransactionsAsArray = function() {
    return this._signedTransactions.slice();
  };
  type.prototype.signedTransactions = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getSignedTransactionsAt(indexOrValues);
      } else {
        this.setSignedTransactionsAt(indexOrValues, value);
      }
    } else {
      this.setSignedTransactions(indexOrValues);
    }
  };
  var signedTransactionsCount = function() {
  console.assert(this._signedTransactions.length == this._fields[1].length);
    return this._signedTransactions.length;
  };
  Object.defineProperties(type.prototype, {
    signedTransactionsCount: { get: signedTransactionsCount },
    signedTransactionsSize: { get: signedTransactionsCount },
    signedTransactionsLength: { get: signedTransactionsCount },
  });
  type.prototype.getSignedTransactionsCount = signedTransactionsCount;
  type.prototype.getSignedTransactionsSize = signedTransactionsCount;
  type.prototype.getSignedTransactionsLength = signedTransactionsCount;
  type.prototype.addSignedTransactions = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(SignedTransaction, value);
    this._signedTransactions.push(msg);
    this._fields[1].push(msg._raw);
  console.assert(this._signedTransactions.length == this._fields[1].length);
  };
  type.prototype.removeSignedTransactions = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
    this._signedTransactions.splice(index, 1);
  console.assert(this._signedTransactions.length == this._fields[1].length);
  };
  type.prototype.clearSignedTransactions = function() {
    this._fields[1].length = 0;
    this._signedTransactions.length = 0;
  console.assert(this._signedTransactions.length == this._fields[1].length);
  };
  return type;
})();

var NodeRequest = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof NodeRequest) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    HANDSHAKE: 1,
    BLOCK_REQUEST: 2,
    HIGHT_REQUEST: 3,
    NAT_TEST: 4,
  
    toString: function(value) {
      switch(value) {
        case 1: return 'HANDSHAKE';
        case 2: return 'BLOCK_REQUEST';
        case 3: return 'HIGHT_REQUEST';
        case 4: return 'NAT_TEST';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(16));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
      this.setMyip(rawArray[0][1]);
      this.setNum(rawArray[0][2]);
      this.setMyhost(rawArray[0][3]);
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? NodeRequest.Type.HANDSHAKE : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = NodeRequest.Type.HANDSHAKE;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  type.prototype.getMyip = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setMyip = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearMyip = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'myip', {
    get: type.prototype.getMyip,
    set: type.prototype.setMyip,
  });

  type.prototype.getNum = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setNum = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearNum = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'num', {
    get: type.prototype.getNum,
    set: type.prototype.setNum,
  });

  type.prototype.getMyhost = function() {
    return typeof this._fields[3] == 'undefined' ? '' : this._fields[3];
  }
  type.prototype.setMyhost = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearMyhost = function() {
    this._fields[3] = '';
  };
  Object.defineProperty(type.prototype, 'myhost', {
    get: type.prototype.getMyhost,
    set: type.prototype.setMyhost,
  });

  return type;
})();

var NodeReply = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._fields[1] = new Array();
  if (values && values.ips && values.ips instanceof Array) {
    this.ips(values.ips);
  }
    if (values instanceof NodeReply) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(17));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setHight(rawArray[0][0]);
      if (rawArray[0][1] instanceof Array) {
        this.ips(rawArray[0][1]);
      }
    }
  };

  type.prototype.getHight = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setHight = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearHight = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'hight', {
    get: type.prototype.getHight,
    set: type.prototype.setHight,
  });

    // Replacement setter
  type.prototype.setIps = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
      this._fields[1] = values.slice();
  };
  // Single value setter
  type.prototype.setIpsAt = function(index, value) {
    this._fields[1][index] = value;
  };
  // Getter
  type.prototype.getIpsAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._fields[1][index];
  };
  type.prototype.getIpsAsArray = function() {
    return this._fields[1].slice();
  };
  type.prototype.ips = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getIpsAt(indexOrValues);
      } else {
        this.setIpsAt(indexOrValues, value);
      }
    } else {
      this.setIps(indexOrValues);
    }
  };
  var ipsCount = function() {
    return this._fields[1].length;
  };
  Object.defineProperties(type.prototype, {
    ipsCount: { get: ipsCount },
    ipsSize: { get: ipsCount },
    ipsLength: { get: ipsCount },
  });
  type.prototype.getIpsCount = ipsCount;
  type.prototype.getIpsSize = ipsCount;
  type.prototype.getIpsLength = ipsCount;
  type.prototype.addIps = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    this._fields[1].push(value);
  };
  type.prototype.resizeIps = function(len) {
    this._fields[1].length = values.length;
  };
  type.prototype.removeIps = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
  };
  type.prototype.clearIps = function() {
    this._fields[1].length = 0;
  };
  return type;
})();

var FantasyPlayerPoints = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyPlayerPoints) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(18));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSeason(rawArray[0][0]);
      this.setWeek(rawArray[0][1]);
      this.setPlayerid(rawArray[0][2]);
      this.setPoints(rawArray[0][3]);
      this.setResult(rawArray[0][4]);
    }
  };

  type.prototype.getSeason = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setSeason = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setWeek = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getPlayerid = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getPoints = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setPoints = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPoints = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'points', {
    get: type.prototype.getPoints,
    set: type.prototype.setPoints,
  });

  type.prototype.getResult = function() {
    return typeof this._fields[4] == 'undefined' ? 0.000000 : this._fields[4];
  }
  type.prototype.setResult = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearResult = function() {
    this._fields[4] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'result', {
    get: type.prototype.getResult,
    set: type.prototype.setResult,
  });

  return type;
})();

var ProjectionTrans = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ProjectionTrans) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(19));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSeason(rawArray[0][0]);
      this.setWeek(rawArray[0][1]);
      this.setPlayerid(rawArray[0][2]);
      this.setPoints(rawArray[0][3]);
    }
  };

  type.prototype.getSeason = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setSeason = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setWeek = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getPlayerid = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getPoints = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setPoints = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPoints = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'points', {
    get: type.prototype.getPoints,
    set: type.prototype.setPoints,
  });

  return type;
})();

var PlayerPoints = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PlayerPoints) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(20));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPlayerid(rawArray[0][0]);
      this.setPoints(rawArray[0][1]);
    }
  };

  type.prototype.getPlayerid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getPoints = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setPoints = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPoints = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'points', {
    get: type.prototype.getPoints,
    set: type.prototype.setPoints,
  });

  return type;
})();

var ProjectionTransBlock = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._playerPoints = new Array();
  this._fields[2] = new Array();
  if (values && values.playerPoints && values.playerPoints instanceof Array) {
    this.playerPoints(values.playerPoints);
  }
    if (values instanceof ProjectionTransBlock) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(21));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSeason(rawArray[0][0]);
      this.setWeek(rawArray[0][1]);
      field = rawArray[0][2];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._playerPoints[i] == 'undefined') {
            var msg = new PlayerPoints();
            this._playerPoints[i] = msg;
            this._fields[2][i] = msg._raw;
          }
          this._playerPoints[i]._mergeFromRawArray(field[i]);
        }
      }
    }
  };

  type.prototype.getSeason = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setSeason = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setWeek = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

    // Replacement setter
  type.prototype.setPlayerPoints = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[2].length = values.length;
     this._playerPoints.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new PlayerPoints(values[i]);
       this._playerPoints[i] = msg;
       this._fields[2][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setPlayerPointsAt = function(index, value) {
    var msg = this._maybeConvertToMessage(PlayerPoints, value);
    this._playerPoints[index] = msg;
    this._fields[2][index] = msg._raw;
  };
  // Getter
  type.prototype.getPlayerPointsAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[2].length < index) {
      throw new RangeError();
    }
    return this._playerPoints[index];
  };
  type.prototype.getPlayerPointsAsArray = function() {
    return this._playerPoints.slice();
  };
  type.prototype.playerPoints = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getPlayerPointsAt(indexOrValues);
      } else {
        this.setPlayerPointsAt(indexOrValues, value);
      }
    } else {
      this.setPlayerPoints(indexOrValues);
    }
  };
  var playerPointsCount = function() {
  console.assert(this._playerPoints.length == this._fields[2].length);
    return this._playerPoints.length;
  };
  Object.defineProperties(type.prototype, {
    playerPointsCount: { get: playerPointsCount },
    playerPointsSize: { get: playerPointsCount },
    playerPointsLength: { get: playerPointsCount },
  });
  type.prototype.getPlayerPointsCount = playerPointsCount;
  type.prototype.getPlayerPointsSize = playerPointsCount;
  type.prototype.getPlayerPointsLength = playerPointsCount;
  type.prototype.addPlayerPoints = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(PlayerPoints, value);
    this._playerPoints.push(msg);
    this._fields[2].push(msg._raw);
  console.assert(this._playerPoints.length == this._fields[2].length);
  };
  type.prototype.removePlayerPoints = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[2].splice(index, 1);
    this._playerPoints.splice(index, 1);
  console.assert(this._playerPoints.length == this._fields[2].length);
  };
  type.prototype.clearPlayerPoints = function() {
    this._fields[2].length = 0;
    this._playerPoints.length = 0;
  console.assert(this._playerPoints.length == this._fields[2].length);
  };
  return type;
})();

var Data = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof Data) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    SCHEDULE: 0,
    PLAYER: 1,
    GAME: 2,
    RESULT: 3,
    MESSAGE: 4,
  
    toString: function(value) {
      switch(value) {
        case 0: return 'SCHEDULE';
        case 1: return 'PLAYER';
        case 2: return 'GAME';
        case 3: return 'RESULT';
        case 4: return 'MESSAGE';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(22));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setVersion(rawArray[0][0]);
      this.setType(rawArray[0][1]);
    }
  };

  type.prototype.getVersion = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setVersion = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearVersion = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'version', {
    get: type.prototype.getVersion,
    set: type.prototype.setVersion,
  });

  type.prototype.getType = function() {
    return typeof this._fields[1] == 'undefined' ? Data.Type.SCHEDULE : this._fields[1];
  }
  type.prototype.setType = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearType = function() {
    this._fields[1] = Data.Type.SCHEDULE;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  return type;
})();

var PlayerData = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PlayerData) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(23));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPlayerid(rawArray[0][0]);
      if (typeof this.playerBase == 'undefined') {
        this.setPlayerBase({});
      }
      this._playerBase._mergeFromRawArray(rawArray[0][1]);
      if (typeof this.playerStatus == 'undefined') {
        this.setPlayerStatus({});
      }
      this._playerStatus._mergeFromRawArray(rawArray[0][2]);
      this.setPlayerGameStatus(rawArray[0][3]);
    }
  };

  type.prototype.getPlayerid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getPlayerBase = function() {
    return this._playerBase;
  }
  type.prototype.setPlayerBase = function(value) {
    var msg = this._maybeConvertToMessage(Q__StaticData__.PlayerBase, value);
    this._playerBase = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearPlayerBase = function() {
    this._fields[1] = undefined;
    this._playerBase = undefined;
  };
  Object.defineProperty(type.prototype, 'playerBase', {
    get: type.prototype.getPlayerBase,
    set: type.prototype.setPlayerBase,
  });

  type.prototype.getPlayerStatus = function() {
    return this._playerStatus;
  }
  type.prototype.setPlayerStatus = function(value) {
    var msg = this._maybeConvertToMessage(Q__StatusData__.PlayerStatus, value);
    this._playerStatus = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearPlayerStatus = function() {
    this._fields[2] = undefined;
    this._playerStatus = undefined;
  };
  Object.defineProperty(type.prototype, 'playerStatus', {
    get: type.prototype.getPlayerStatus,
    set: type.prototype.setPlayerStatus,
  });

  type.prototype.getPlayerGameStatus = function() {
    return typeof this._fields[3] == 'undefined' ? PlayerGameStatus.OUT : this._fields[3];
  }
  type.prototype.setPlayerGameStatus = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPlayerGameStatus = function() {
    this._fields[3] = PlayerGameStatus.OUT;
  };
  Object.defineProperty(type.prototype, 'playerGameStatus', {
    get: type.prototype.getPlayerGameStatus,
    set: type.prototype.setPlayerGameStatus,
  });

  return type;
})();

var GameData = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GameData) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(24));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setGameid(rawArray[0][0]);
      if (typeof this.status == 'undefined') {
        this.setStatus({});
      }
      this._status._mergeFromRawArray(rawArray[0][1]);
    }
  };

  type.prototype.getGameid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setGameid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearGameid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'gameid', {
    get: type.prototype.getGameid,
    set: type.prototype.setGameid,
  });

  type.prototype.getStatus = function() {
    return this._status;
  }
  type.prototype.setStatus = function(value) {
    var msg = this._maybeConvertToMessage(Q__StatusData__.GameStatus, value);
    this._status = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearStatus = function() {
    this._fields[1] = undefined;
    this._status = undefined;
  };
  Object.defineProperty(type.prototype, 'status', {
    get: type.prototype.getStatus,
    set: type.prototype.setStatus,
  });

  return type;
})();

var ResultData = (function() {
  var type = function(values) {
    this._fields = new Array(1);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ResultData) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(25));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      if (typeof this.gameResult == 'undefined') {
        this.setGameResult({});
      }
      this._gameResult._mergeFromRawArray(rawArray[0][0]);
    }
  };

  type.prototype.getGameResult = function() {
    return this._gameResult;
  }
  type.prototype.setGameResult = function(value) {
    var msg = this._maybeConvertToMessage(Q__StaticData__.GameResult, value);
    this._gameResult = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearGameResult = function() {
    this._fields[0] = undefined;
    this._gameResult = undefined;
  };
  Object.defineProperty(type.prototype, 'gameResult', {
    get: type.prototype.getGameResult,
    set: type.prototype.setGameResult,
  });

  return type;
})();

var ScheduleData = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ScheduleData) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(26));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setWeek(rawArray[0][0]);
      if (typeof this.weekly == 'undefined') {
        this.setWeekly({});
      }
      this._weekly._mergeFromRawArray(rawArray[0][1]);
    }
  };

  type.prototype.getWeek = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setWeek = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getWeekly = function() {
    return this._weekly;
  }
  type.prototype.setWeekly = function(value) {
    var msg = this._maybeConvertToMessage(Q__StaticData__.WeeklySchedule, value);
    this._weekly = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearWeekly = function() {
    this._fields[1] = undefined;
    this._weekly = undefined;
  };
  Object.defineProperty(type.prototype, 'weekly', {
    get: type.prototype.getWeekly,
    set: type.prototype.setWeekly,
  });

  return type;
})();

var DataTransition = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._gamedata = new Array();
  this._fields[3] = new Array();
  if (values && values.gamedata && values.gamedata instanceof Array) {
    this.gamedata(values.gamedata);
  }
  this._data = new Array();
  this._fields[4] = new Array();
  if (values && values.data && values.data instanceof Array) {
    this.data(values.data);
  }
    if (values instanceof DataTransition) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(27));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
      this.setSeason(rawArray[0][1]);
      this.setWeek(rawArray[0][2]);
      field = rawArray[0][3];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._gamedata[i] == 'undefined') {
            var msg = new GameData();
            this._gamedata[i] = msg;
            this._fields[3][i] = msg._raw;
          }
          this._gamedata[i]._mergeFromRawArray(field[i]);
        }
      }
      field = rawArray[0][4];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._data[i] == 'undefined') {
            var msg = new Data();
            this._data[i] = msg;
            this._fields[4][i] = msg._raw;
          }
          this._data[i]._mergeFromRawArray(field[i]);
        }
      }
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? TrType.SEASONSTART : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = TrType.SEASONSTART;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  type.prototype.getSeason = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSeason = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setWeek = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

    // Replacement setter
  type.prototype.setGamedata = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[3].length = values.length;
     this._gamedata.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new GameData(values[i]);
       this._gamedata[i] = msg;
       this._fields[3][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setGamedataAt = function(index, value) {
    var msg = this._maybeConvertToMessage(GameData, value);
    this._gamedata[index] = msg;
    this._fields[3][index] = msg._raw;
  };
  // Getter
  type.prototype.getGamedataAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[3].length < index) {
      throw new RangeError();
    }
    return this._gamedata[index];
  };
  type.prototype.getGamedataAsArray = function() {
    return this._gamedata.slice();
  };
  type.prototype.gamedata = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getGamedataAt(indexOrValues);
      } else {
        this.setGamedataAt(indexOrValues, value);
      }
    } else {
      this.setGamedata(indexOrValues);
    }
  };
  var gamedataCount = function() {
  console.assert(this._gamedata.length == this._fields[3].length);
    return this._gamedata.length;
  };
  Object.defineProperties(type.prototype, {
    gamedataCount: { get: gamedataCount },
    gamedataSize: { get: gamedataCount },
    gamedataLength: { get: gamedataCount },
  });
  type.prototype.getGamedataCount = gamedataCount;
  type.prototype.getGamedataSize = gamedataCount;
  type.prototype.getGamedataLength = gamedataCount;
  type.prototype.addGamedata = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(GameData, value);
    this._gamedata.push(msg);
    this._fields[3].push(msg._raw);
  console.assert(this._gamedata.length == this._fields[3].length);
  };
  type.prototype.removeGamedata = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[3].splice(index, 1);
    this._gamedata.splice(index, 1);
  console.assert(this._gamedata.length == this._fields[3].length);
  };
  type.prototype.clearGamedata = function() {
    this._fields[3].length = 0;
    this._gamedata.length = 0;
  console.assert(this._gamedata.length == this._fields[3].length);
  };
    // Replacement setter
  type.prototype.setData = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[4].length = values.length;
     this._data.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new Data(values[i]);
       this._data[i] = msg;
       this._fields[4][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setDataAt = function(index, value) {
    var msg = this._maybeConvertToMessage(Data, value);
    this._data[index] = msg;
    this._fields[4][index] = msg._raw;
  };
  // Getter
  type.prototype.getDataAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[4].length < index) {
      throw new RangeError();
    }
    return this._data[index];
  };
  type.prototype.getDataAsArray = function() {
    return this._data.slice();
  };
  type.prototype.data = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getDataAt(indexOrValues);
      } else {
        this.setDataAt(indexOrValues, value);
      }
    } else {
      this.setData(indexOrValues);
    }
  };
  var dataCount = function() {
  console.assert(this._data.length == this._fields[4].length);
    return this._data.length;
  };
  Object.defineProperties(type.prototype, {
    dataCount: { get: dataCount },
    dataSize: { get: dataCount },
    dataLength: { get: dataCount },
  });
  type.prototype.getDataCount = dataCount;
  type.prototype.getDataSize = dataCount;
  type.prototype.getDataLength = dataCount;
  type.prototype.addData = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(Data, value);
    this._data.push(msg);
    this._fields[4].push(msg._raw);
  console.assert(this._data.length == this._fields[4].length);
  };
  type.prototype.removeData = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[4].splice(index, 1);
    this._data.splice(index, 1);
  console.assert(this._data.length == this._fields[4].length);
  };
  type.prototype.clearData = function() {
    this._fields[4].length = 0;
    this._data.length = 0;
  console.assert(this._data.length == this._fields[4].length);
  };
  return type;
})();

var TeamState = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TeamState) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var State = {
    PREGAME: 1,
    INGAME: 2,
  
    toString: function(value) {
      switch(value) {
        case 1: return 'PREGAME';
        case 2: return 'INGAME';
      }
    },
  };
  type.State = State;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(28));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setState(rawArray[0][0]);
      this.setWeek(rawArray[0][1]);
      this.setTeamid(rawArray[0][2]);
    }
  };

  type.prototype.getState = function() {
    return typeof this._fields[0] == 'undefined' ? TeamState.State.PREGAME : this._fields[0];
  }
  type.prototype.setState = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearState = function() {
    this._fields[0] = TeamState.State.PREGAME;
  };
  Object.defineProperty(type.prototype, 'state', {
    get: type.prototype.getState,
    set: type.prototype.setState,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setWeek = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getTeamid = function() {
    return typeof this._fields[2] == 'undefined' ? '' : this._fields[2];
  }
  type.prototype.setTeamid = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearTeamid = function() {
    this._fields[2] = '';
  };
  Object.defineProperty(type.prototype, 'teamid', {
    get: type.prototype.getTeamid,
    set: type.prototype.setTeamid,
  });

  return type;
})();

var DeltaData = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._myfantasyname = new Array();
  this._fields[1] = new Array();
  if (values && values.myfantasyname && values.myfantasyname instanceof Array) {
    this.myfantasyname(values.myfantasyname);
  }
  this._teamstates = new Array();
  this._fields[3] = new Array();
  if (values && values.teamstates && values.teamstates instanceof Array) {
    this.teamstates(values.teamstates);
  }
  this._datas = new Array();
  this._fields[4] = new Array();
  if (values && values.datas && values.datas instanceof Array) {
    this.datas(values.datas);
  }
  this._players = new Array();
  this._fields[5] = new Array();
  if (values && values.players && values.players instanceof Array) {
    this.players(values.players);
  }
    if (values instanceof DeltaData) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    SNAPSHOT: 2,
    HEARTBEAT: 3,
  
    toString: function(value) {
      switch(value) {
        case 2: return 'SNAPSHOT';
        case 3: return 'HEARTBEAT';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(29));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
      field = rawArray[0][1];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._myfantasyname[i] == 'undefined') {
            var msg = new MyFantasyName();
            this._myfantasyname[i] = msg;
            this._fields[1][i] = msg._raw;
          }
          this._myfantasyname[i]._mergeFromRawArray(field[i]);
        }
      }
      if (typeof this.globalstate == 'undefined') {
        this.setGlobalstate({});
      }
      this._globalstate._mergeFromRawArray(rawArray[0][2]);
      field = rawArray[0][3];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._teamstates[i] == 'undefined') {
            var msg = new TeamState();
            this._teamstates[i] = msg;
            this._fields[3][i] = msg._raw;
          }
          this._teamstates[i]._mergeFromRawArray(field[i]);
        }
      }
      field = rawArray[0][4];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._datas[i] == 'undefined') {
            var msg = new Data();
            this._datas[i] = msg;
            this._fields[4][i] = msg._raw;
          }
          this._datas[i]._mergeFromRawArray(field[i]);
        }
      }
      field = rawArray[0][5];
      if (field instanceof Array) {
        for (var i = 0; i < field.length; ++i) {
          if (typeof this._players[i] == 'undefined') {
            var msg = new FantasyPlayer();
            this._players[i] = msg;
            this._fields[5][i] = msg._raw;
          }
          this._players[i]._mergeFromRawArray(field[i]);
        }
      }
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? DeltaData.Type.SNAPSHOT : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = DeltaData.Type.SNAPSHOT;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

    // Replacement setter
  type.prototype.setMyfantasyname = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[1].length = values.length;
     this._myfantasyname.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new MyFantasyName(values[i]);
       this._myfantasyname[i] = msg;
       this._fields[1][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setMyfantasynameAt = function(index, value) {
    var msg = this._maybeConvertToMessage(MyFantasyName, value);
    this._myfantasyname[index] = msg;
    this._fields[1][index] = msg._raw;
  };
  // Getter
  type.prototype.getMyfantasynameAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._myfantasyname[index];
  };
  type.prototype.getMyfantasynameAsArray = function() {
    return this._myfantasyname.slice();
  };
  type.prototype.myfantasyname = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getMyfantasynameAt(indexOrValues);
      } else {
        this.setMyfantasynameAt(indexOrValues, value);
      }
    } else {
      this.setMyfantasyname(indexOrValues);
    }
  };
  var myfantasynameCount = function() {
  console.assert(this._myfantasyname.length == this._fields[1].length);
    return this._myfantasyname.length;
  };
  Object.defineProperties(type.prototype, {
    myfantasynameCount: { get: myfantasynameCount },
    myfantasynameSize: { get: myfantasynameCount },
    myfantasynameLength: { get: myfantasynameCount },
  });
  type.prototype.getMyfantasynameCount = myfantasynameCount;
  type.prototype.getMyfantasynameSize = myfantasynameCount;
  type.prototype.getMyfantasynameLength = myfantasynameCount;
  type.prototype.addMyfantasyname = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(MyFantasyName, value);
    this._myfantasyname.push(msg);
    this._fields[1].push(msg._raw);
  console.assert(this._myfantasyname.length == this._fields[1].length);
  };
  type.prototype.removeMyfantasyname = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
    this._myfantasyname.splice(index, 1);
  console.assert(this._myfantasyname.length == this._fields[1].length);
  };
  type.prototype.clearMyfantasyname = function() {
    this._fields[1].length = 0;
    this._myfantasyname.length = 0;
  console.assert(this._myfantasyname.length == this._fields[1].length);
  };
  type.prototype.getGlobalstate = function() {
    return this._globalstate;
  }
  type.prototype.setGlobalstate = function(value) {
    var msg = this._maybeConvertToMessage(Q__StatusData__.GlobalState, value);
    this._globalstate = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearGlobalstate = function() {
    this._fields[2] = undefined;
    this._globalstate = undefined;
  };
  Object.defineProperty(type.prototype, 'globalstate', {
    get: type.prototype.getGlobalstate,
    set: type.prototype.setGlobalstate,
  });

    // Replacement setter
  type.prototype.setTeamstates = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[3].length = values.length;
     this._teamstates.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new TeamState(values[i]);
       this._teamstates[i] = msg;
       this._fields[3][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setTeamstatesAt = function(index, value) {
    var msg = this._maybeConvertToMessage(TeamState, value);
    this._teamstates[index] = msg;
    this._fields[3][index] = msg._raw;
  };
  // Getter
  type.prototype.getTeamstatesAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[3].length < index) {
      throw new RangeError();
    }
    return this._teamstates[index];
  };
  type.prototype.getTeamstatesAsArray = function() {
    return this._teamstates.slice();
  };
  type.prototype.teamstates = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getTeamstatesAt(indexOrValues);
      } else {
        this.setTeamstatesAt(indexOrValues, value);
      }
    } else {
      this.setTeamstates(indexOrValues);
    }
  };
  var teamstatesCount = function() {
  console.assert(this._teamstates.length == this._fields[3].length);
    return this._teamstates.length;
  };
  Object.defineProperties(type.prototype, {
    teamstatesCount: { get: teamstatesCount },
    teamstatesSize: { get: teamstatesCount },
    teamstatesLength: { get: teamstatesCount },
  });
  type.prototype.getTeamstatesCount = teamstatesCount;
  type.prototype.getTeamstatesSize = teamstatesCount;
  type.prototype.getTeamstatesLength = teamstatesCount;
  type.prototype.addTeamstates = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(TeamState, value);
    this._teamstates.push(msg);
    this._fields[3].push(msg._raw);
  console.assert(this._teamstates.length == this._fields[3].length);
  };
  type.prototype.removeTeamstates = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[3].splice(index, 1);
    this._teamstates.splice(index, 1);
  console.assert(this._teamstates.length == this._fields[3].length);
  };
  type.prototype.clearTeamstates = function() {
    this._fields[3].length = 0;
    this._teamstates.length = 0;
  console.assert(this._teamstates.length == this._fields[3].length);
  };
    // Replacement setter
  type.prototype.setDatas = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[4].length = values.length;
     this._datas.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new Data(values[i]);
       this._datas[i] = msg;
       this._fields[4][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setDatasAt = function(index, value) {
    var msg = this._maybeConvertToMessage(Data, value);
    this._datas[index] = msg;
    this._fields[4][index] = msg._raw;
  };
  // Getter
  type.prototype.getDatasAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[4].length < index) {
      throw new RangeError();
    }
    return this._datas[index];
  };
  type.prototype.getDatasAsArray = function() {
    return this._datas.slice();
  };
  type.prototype.datas = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getDatasAt(indexOrValues);
      } else {
        this.setDatasAt(indexOrValues, value);
      }
    } else {
      this.setDatas(indexOrValues);
    }
  };
  var datasCount = function() {
  console.assert(this._datas.length == this._fields[4].length);
    return this._datas.length;
  };
  Object.defineProperties(type.prototype, {
    datasCount: { get: datasCount },
    datasSize: { get: datasCount },
    datasLength: { get: datasCount },
  });
  type.prototype.getDatasCount = datasCount;
  type.prototype.getDatasSize = datasCount;
  type.prototype.getDatasLength = datasCount;
  type.prototype.addDatas = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(Data, value);
    this._datas.push(msg);
    this._fields[4].push(msg._raw);
  console.assert(this._datas.length == this._fields[4].length);
  };
  type.prototype.removeDatas = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[4].splice(index, 1);
    this._datas.splice(index, 1);
  console.assert(this._datas.length == this._fields[4].length);
  };
  type.prototype.clearDatas = function() {
    this._fields[4].length = 0;
    this._datas.length = 0;
  console.assert(this._datas.length == this._fields[4].length);
  };
    // Replacement setter
  type.prototype.setPlayers = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
    this._fields[5].length = values.length;
     this._players.length = values.length;
     for (var i = 0; i < values.length; ++i) {
       var msg = new FantasyPlayer(values[i]);
       this._players[i] = msg;
       this._fields[5][i] = msg._raw;
     }
  };
  // Single value setter
  type.prototype.setPlayersAt = function(index, value) {
    var msg = this._maybeConvertToMessage(FantasyPlayer, value);
    this._players[index] = msg;
    this._fields[5][index] = msg._raw;
  };
  // Getter
  type.prototype.getPlayersAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[5].length < index) {
      throw new RangeError();
    }
    return this._players[index];
  };
  type.prototype.getPlayersAsArray = function() {
    return this._players.slice();
  };
  type.prototype.players = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getPlayersAt(indexOrValues);
      } else {
        this.setPlayersAt(indexOrValues, value);
      }
    } else {
      this.setPlayers(indexOrValues);
    }
  };
  var playersCount = function() {
  console.assert(this._players.length == this._fields[5].length);
    return this._players.length;
  };
  Object.defineProperties(type.prototype, {
    playersCount: { get: playersCount },
    playersSize: { get: playersCount },
    playersLength: { get: playersCount },
  });
  type.prototype.getPlayersCount = playersCount;
  type.prototype.getPlayersSize = playersCount;
  type.prototype.getPlayersLength = playersCount;
  type.prototype.addPlayers = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    var msg = this._maybeConvertToMessage(FantasyPlayer, value);
    this._players.push(msg);
    this._fields[5].push(msg._raw);
  console.assert(this._players.length == this._fields[5].length);
  };
  type.prototype.removePlayers = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[5].splice(index, 1);
    this._players.splice(index, 1);
  console.assert(this._players.length == this._fields[5].length);
  };
  type.prototype.clearPlayers = function() {
    this._fields[5].length = 0;
    this._players.length = 0;
  console.assert(this._players.length == this._fields[5].length);
  };
  return type;
})();

var MessageData = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof MessageData) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(30));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setMsg(rawArray[0][0]);
      this.setGt(rawArray[0][1]);
      this.setLt(rawArray[0][2]);
    }
  };

  type.prototype.getMsg = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setMsg = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearMsg = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'msg', {
    get: type.prototype.getMsg,
    set: type.prototype.setMsg,
  });

  type.prototype.getGt = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setGt = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearGt = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'gt', {
    get: type.prototype.getGt,
    set: type.prototype.setGt,
  });

  type.prototype.getLt = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setLt = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearLt = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'lt', {
    get: type.prototype.getLt,
    set: type.prototype.setLt,
  });

  return type;
})();

var StampedTrans = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof StampedTrans) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(31));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setTimestamp(rawArray[0][0]);
      this.setSeqnum(rawArray[0][1]);
      this.setPrevseq(rawArray[0][2]);
      if (typeof this.signedOrig == 'undefined') {
        this.setSignedOrig({});
      }
      this._signedOrig._mergeFromRawArray(rawArray[0][3]);
    }
  };

  type.prototype.getTimestamp = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setTimestamp = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTimestamp = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'timestamp', {
    get: type.prototype.getTimestamp,
    set: type.prototype.setTimestamp,
  });

  type.prototype.getSeqnum = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSeqnum = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSeqnum = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'seqnum', {
    get: type.prototype.getSeqnum,
    set: type.prototype.setSeqnum,
  });

  type.prototype.getPrevseq = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setPrevseq = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrevseq = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'prevseq', {
    get: type.prototype.getPrevseq,
    set: type.prototype.setPrevseq,
  });

  type.prototype.getSignedOrig = function() {
    return this._signedOrig;
  }
  type.prototype.setSignedOrig = function(value) {
    var msg = this._maybeConvertToMessage(SignedTransaction, value);
    this._signedOrig = msg;
    this._fields[3] = msg._raw;
  };
  type.prototype.clearSignedOrig = function() {
    this._fields[3] = undefined;
    this._signedOrig = undefined;
  };
  Object.defineProperty(type.prototype, 'signedOrig', {
    get: type.prototype.getSignedOrig,
    set: type.prototype.setSignedOrig,
  });

  return type;
})();

var ExchangeOrder = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ExchangeOrder) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    NEW: 1,
    CANCEL: 2,
    REPLACE: 3,
  
    toString: function(value) {
      switch(value) {
        case 1: return 'NEW';
        case 2: return 'CANCEL';
        case 3: return 'REPLACE';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(32));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
      this.setPlayerid(rawArray[0][1]);
      if (typeof this.core == 'undefined') {
        this.setCore({});
      }
      this._core._mergeFromRawArray(rawArray[0][2]);
      this.setCancelOref(rawArray[0][3]);
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? ExchangeOrder.Type.NEW : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = ExchangeOrder.Type.NEW;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  type.prototype.getPlayerid = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getCore = function() {
    return this._core;
  }
  type.prototype.setCore = function(value) {
    var msg = this._maybeConvertToMessage(Q__ExData__.OrderCore, value);
    this._core = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearCore = function() {
    this._fields[2] = undefined;
    this._core = undefined;
  };
  Object.defineProperty(type.prototype, 'core', {
    get: type.prototype.getCore,
    set: type.prototype.setCore,
  });

  type.prototype.getCancelOref = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setCancelOref = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearCancelOref = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'cancelOref', {
    get: type.prototype.getCancelOref,
    set: type.prototype.setCancelOref,
  });

  return type;
})();

var OrderUnique = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof OrderUnique) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(33));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPublicKey(rawArray[0][0]);
      this.setSeason(rawArray[0][1]);
      this.setWeek(rawArray[0][2]);
      this.setBuyside(rawArray[0][3]);
      this.setPlayerid(rawArray[0][4]);
      this.setPrice(rawArray[0][5]);
    }
  };

  type.prototype.getPublicKey = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPublicKey = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPublicKey = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'publicKey', {
    get: type.prototype.getPublicKey,
    set: type.prototype.setPublicKey,
  });

  type.prototype.getSeason = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSeason = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setWeek = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getBuyside = function() {
    return typeof this._fields[3] == 'undefined' ? false : this._fields[3];
  }
  type.prototype.setBuyside = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearBuyside = function() {
    this._fields[3] = false;
  };
  Object.defineProperty(type.prototype, 'buyside', {
    get: type.prototype.getBuyside,
    set: type.prototype.setBuyside,
  });

  type.prototype.getPlayerid = function() {
    return typeof this._fields[4] == 'undefined' ? '' : this._fields[4];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[4] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[5] == 'undefined' ? 0 : this._fields[5];
  }
  type.prototype.setPrice = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[5] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  return type;
})();

var OrderDeets = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof OrderDeets) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(34));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setOid(rawArray[0][0]);
      if (typeof this.order == 'undefined') {
        this.setOrder({});
      }
      this._order._mergeFromRawArray(rawArray[0][1]);
      this.setSize(rawArray[0][2]);
    }
  };

  type.prototype.getOid = function() {
    return typeof this._fields[0] == 'undefined' ?  new ArrayBuffer() : this._fields[0];
  }
  type.prototype.setOid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearOid = function() {
    this._fields[0] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'oid', {
    get: type.prototype.getOid,
    set: type.prototype.setOid,
  });

  type.prototype.getOrder = function() {
    return this._order;
  }
  type.prototype.setOrder = function(value) {
    var msg = this._maybeConvertToMessage(OrderUnique, value);
    this._order = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearOrder = function() {
    this._fields[1] = undefined;
    this._order = undefined;
  };
  Object.defineProperty(type.prototype, 'order', {
    get: type.prototype.getOrder,
    set: type.prototype.setOrder,
  });

  type.prototype.getSize = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setSize = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearSize = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'size', {
    get: type.prototype.getSize,
    set: type.prototype.setSize,
  });

  return type;
})();

var InsideUnique = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof InsideUnique) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(35));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPlayerid(rawArray[0][0]);
      this.setPublicKey(rawArray[0][1]);
      this.setBuyside(rawArray[0][2]);
      this.setPrice(rawArray[0][3]);
      this.setOid(rawArray[0][4]);
      if (typeof this.order == 'undefined') {
        this.setOrder({});
      }
      this._order._mergeFromRawArray(rawArray[0][5]);
      this.setSize(rawArray[0][6]);
    }
  };

  type.prototype.getPlayerid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getPublicKey = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setPublicKey = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPublicKey = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'publicKey', {
    get: type.prototype.getPublicKey,
    set: type.prototype.setPublicKey,
  });

  type.prototype.getBuyside = function() {
    return typeof this._fields[2] == 'undefined' ? false : this._fields[2];
  }
  type.prototype.setBuyside = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearBuyside = function() {
    this._fields[2] = false;
  };
  Object.defineProperty(type.prototype, 'buyside', {
    get: type.prototype.getBuyside,
    set: type.prototype.setBuyside,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setPrice = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  type.prototype.getOid = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setOid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearOid = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'oid', {
    get: type.prototype.getOid,
    set: type.prototype.setOid,
  });

  type.prototype.getOrder = function() {
    return this._order;
  }
  type.prototype.setOrder = function(value) {
    var msg = this._maybeConvertToMessage(OrderUnique, value);
    this._order = msg;
    this._fields[5] = msg._raw;
  };
  type.prototype.clearOrder = function() {
    this._fields[5] = undefined;
    this._order = undefined;
  };
  Object.defineProperty(type.prototype, 'order', {
    get: type.prototype.getOrder,
    set: type.prototype.setOrder,
  });

  type.prototype.getSize = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setSize = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearSize = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'size', {
    get: type.prototype.getSize,
    set: type.prototype.setSize,
  });

  return type;
})();

var TimeTransition = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TimeTransition) {
      this._mergeFromRawArray(values._raw);
    } else if (values instanceof Array) {
      this._mergeFromRawArray(values);
    } else {
      for (var k in values) {
        if (this[k] instanceof Function) {
          this[k](values[k]);
        } else {
          this[k] = values[k];
        }
      }
    }
  };

  
  var Type = {
    TRADEOPEN: 2,
    TRADECLOSE: 3,
  
    toString: function(value) {
      switch(value) {
        case 2: return 'TRADEOPEN';
        case 3: return 'TRADECLOSE';
      }
    },
  };
  type.Type = Type;
  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(36));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setType(rawArray[0][0]);
      this.setSeason(rawArray[0][1]);
      this.setWeek(rawArray[0][2]);
    }
  };

  type.prototype.getType = function() {
    return typeof this._fields[0] == 'undefined' ? TimeTransition.Type.TRADEOPEN : this._fields[0];
  }
  type.prototype.setType = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearType = function() {
    this._fields[0] = TimeTransition.Type.TRADEOPEN;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  type.prototype.getSeason = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSeason = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setWeek = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  return type;
})();

