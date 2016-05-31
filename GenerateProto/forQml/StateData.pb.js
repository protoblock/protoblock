.pragma library
.import Protobuf 1.0 as Protobuf
.import 'ProtoData.pb.js' as Q__ProtoData__
.import 'NameData.pb.js' as Q__NameData__
.import 'StaticData.pb.js' as Q__StaticData__
.import 'StatusData.pb.js' as Q__StatusData__
.import 'ExData.pb.js' as Q__ExData__
'use strict';

var _file = {
  get descriptor() {
    if(!this._desc && Q__ProtoData__._file.descriptor && Q__NameData__._file.descriptor && Q__StaticData__._file.descriptor && Q__StatusData__._file.descriptor && Q__ExData__._file.descriptor) {
      this._desc = Protobuf.DescriptorPool.addFileDescriptor('Cg9TdGF0ZURhdGEucHJvdG8SCmZhbnRhc3liaXQaD1Byb3RvRGF0YS5wcm90bxoOTmFtZURhdGEucHJvdG8aEFN0YXRpY0RhdGEucHJvdG8aEFN0YXR1c0RhdGEucHJvdG8aDEV4RGF0YS5wcm90byJyCglCbG9ja01ldGESEAoIYmxvY2tudW0YKCABKAUSEAoIdHJtZXRhaWQYMiABKAwSEgoKdHhtZXRhcm9vdBgKIAEoDBIRCglwYnN0YXRlaWQYPCABKAwSDAoEcHJldhgUIAEoDBIMCgRuZXh0GB4gASgMIpcBCgZUeE1ldGESEAoIYmxvY2tudW0YCiABKAUSDQoFdHhudW0YFCABKAUSJQoGdHh0eXBlGCggASgOMhUuZmFudGFzeWJpdC5UcmFuc1R5cGUSIwoCdHgYMiABKAsyFy5mYW50YXN5Yml0LlRyYW5zYWN0aW9uEhQKDGZhbnRhc3lfbmFtZRg8IAEoCRIKCgJpZBhGIAEoDCKYAQoGVHJNZXRhEhAKCGJsb2NrbnVtGAogASgFEiAKBHR5cGUYKCABKA4yEi5mYW50YXN5Yml0LlRyVHlwZRIOCgZzZWFzb24YMiABKA0SDAoEd2Vlaxg8IAEoDRIUCgxnYW1lbWV0YXJvb3QYRiABKAwSFAoMZGF0YW1ldGFyb290GFAgASgMEhAKCHR4bWV0YWlkGGQgASgMIkQKCEdhbWVNZXRhEhAKCHR4bWV0YWlkGAogASgMEiYKCGdhbWVkYXRhGBQgASgLMhQuZmFudGFzeWJpdC5HYW1lRGF0YSI8CghEYXRhTWV0YRIQCgh0eG1ldGFpZBgKIAEoDBIeCgRkYXRhGBQgASgLMhAuZmFudGFzeWJpdC5EYXRhIlIKD0xlYWRlckJvYXJkTWV0YRIMCgR3ZWVrGAogASgNEhUKDWZuYmFsbWV0YXJvb3QYFCABKAwSDAoEcHJldhgeIAEoDBIMCgRuZXh0GCggASgMIq4BChJGYW50YXN5TmFtZUJhbE1ldGESEAoIdHhtZXRhaWQYCiABKAwSEAoIdHJtZXRhaWQYFCABKAwSLAoIZm5hbWViYWwYHiABKAsyGi5mYW50YXN5Yml0LkZhbnRhc3lOYW1lQmFsEhUKDWF3YXJkbWV0YXJvb3QYMiABKAwSEwoLcG5sbWV0YXJvb3QYPCABKAwSDAoEcHJldhhGIAEoDBIMCgRuZXh0GFAgASgMIq8BChBGYW50YXN5TmFtZVN0YXRlEgwKBG5hbWUYASABKAkSHAoUZmFudGFzeW5hbWViYWxtZXRhaWQYCiABKAwSEwoLcG9zbWV0YXJvb3QYFCABKAwSFgoOb3JkZXJzbWV0YXJvb3QYHiABKAwSFAoMcHJvam1ldGFyb290GCggASgMEgwKBHByZXYYRiABKAwSDAoEbmV4dBhQIAEoDBIQCgh0eG1ldGFpZBgyIAEoDCKSAQoSV2Vla0dhbWVTdGF0dXNNZXRhEgwKBHdlZWsYCiABKA0SGgoSZ2FtZXJlc3VsdG1ldGFyb290GBQgASgMEhoKEmluZ2FtZXByb2ptZXRhcm9vdBgeIAEoDBIaChJvcGVuZ2FtZXN0YXR1c3Jvb3QYKCABKAwSDAoEcHJldhgyIAEoDBIMCgRuZXh0GDwgASgMIn4KDkluR2FtZVByb2pNZXRhEg4KBmdhbWVpZBgUIAEoCRIYChBnYW1lc3RhdHVzbWV0YWlkGCggASgMEhQKDGhvbWVwcm9qbWV0YRg8IAEoDBIUCgxhd2F5cHJvam1ldGEYRiABKAwSFgoOZ2FtZWRhdGFtZXRhaWQYMiABKAwihQEKD0dhbWVSZXN1bHRzTWV0YRIOCgZnYW1laWQYFCABKAkSGAoQZ2FtZXN0YXR1c21ldGFpZBgoIAEoDBIWCg5ob21lcmVzdWx0bWV0YRg8IAEoDBIWCg5hd2F5cmVzdWx0bWV0YRhGIAEoDBIYChByZXN1bHRkYXRhbWV0YWlkGDIgASgMIsEBCg5HYW1lU3RhdHVzTWV0YRIKCgJpZBgBIAEoCRIMCgR3ZWVrGAIgASgFEiYKCGdhbWVpbmZvGAogASgLMhQuZmFudGFzeWJpdC5HYW1lSW5mbxIpCglnYW1lc2F0dXMYFCABKAsyFi5mYW50YXN5Yml0LkdhbWVTdGF0dXMSEgoKZ2FtZW1ldGFpZBgeIAEoDBISCgpkYXRhbWV0YWlkGCggASgMEgwKBHByZXYYMiABKAwSDAoEbmV4dBg8IAEoDCKHAQoMVGVhbVByb2pNZXRhEg4KBmdhbWVpZBgUIAEoCRIMCgR0ZWFtGB4gASgJEhMKC2tpY2tvZmZ0aW1lGCggASgNEgwKBHdlZWsYMiABKA0SHgoWZ2FtZXBsYXllcnByb2ptZXRhcm9vdBg8IAEoDBIWCg5nYW1lZGF0YW1ldGFpZBhGIAEoDCKJAQoOVGVhbVJlc3VsdE1ldGESDgoGZ2FtZWlkGBQgASgJEgwKBHRlYW0YHiABKAkSEwoLa2lja29mZnRpbWUYKCABKA0SDAoEd2VlaxgyIAEoDRIcChRwbGF5ZXJyZXN1bHRtZXRhcm9vdBg8IAEoDBIYChByZXN1bHRkYXRhbWV0YWlkGEYgASgMIo8BChJHYW1lUGxheWVyUHJvak1ldGESEAoIcGxheWVyaWQYFCABKAkSGAoQZ2FtZXN0YXR1c21ldGFpZBgeIAEoDBIaChJwcm9qbWV0YXBsYXllcnJvb3QYPCABKAwSGQoRcG9zbWV0YXBsYXllcnJvb3QYRiABKAwSFgoOZ2FtZWRhdGFtZXRhaWQYUCABKAwiWAoIUHJvak1ldGESDAoEbmFtZRgKIAEoCRIMCgRwcm9qGBQgASgFEhAKCHBsYXllcmlkGB4gASgJEhAKCHR4bWV0YWlkGCggASgMEgwKBHByZXYYMiABKAwiZQoHUG9zTWV0YRIMCgRuYW1lGAogASgJEgsKA3F0eRgUIAEoBRINCgVwcmljZRgeIAEoBRIQCghwbGF5ZXJpZBgoIAEoCRIQCgh0eG1ldGFpZBgyIAEoDBIMCgRwcmV2GDwgASgMIsIBChBQbGF5ZXJSZXN1bHRNZXRhEhAKCHBsYXllcmlkGBQgASgJEhgKEGdhbWVzdGF0dXNtZXRhaWQYHiABKAwSDgoGcmVzdWx0GCggASgCEiAKBXN0YXRzGDIgASgLMhEuZmFudGFzeWJpdC5TdGF0cxIbChNhd2FyZG1ldGFwbGF5ZXJyb290GDwgASgMEhkKEXBubG1ldGFwbGF5ZXJyb290GEYgASgMEhgKEHJlc3VsdGRhdGFtZXRhaWQYUCABKAwiZAoJQXdhcmRNZXRhEgwKBG5hbWUYCiABKAkSDAoEcHJvahgUIAEoBRINCgVhd2FyZBgeIAEoBBISCgpwcm9qbWV0YWlkGCggASgMEhgKEHJlc3VsdGRhdGFtZXRhaWQYMiABKAwiaAoHUG5sTWV0YRIvCgxmYW50YXliaXRwbmwYFCABKAsyGS5mYW50YXN5Yml0LkZhbnRhc3lCaXRQbmwSEgoKcHJvam1ldGFpZBgeIAEoDBIYChByZXN1bHRkYXRhbWV0YWlkGCggASgMInMKEVBsYXllck1hcmtldFN0YXRlEhcKD2xpbWl0Ym9va21ldGFpZBgKIAEoDBIQCghwbGF5ZXJpZBgUIAEoCRIXCg9tYXJrZXR0aWNtZXRhaWQYHiABKAwSDAoEcHJldhgoIAEoDBIMCgRuZXh0GDIgASgMIn8KDUxpbWl0Qm9va01ldGESCwoDYmlkGAogASgFEg8KB2JpZHNpemUYFCABKAUSDwoHYXNrc2l6ZRgeIAEoBRILCgNhc2sYKCABKAUSGAoQYmlkb3JkZXJtZXRhcm9vdBgyIAEoDBIYChBhc2tvcmRlcm1ldGFyb290GDwgASgMIlEKDU1hcmtldFRpY01ldGESDQoFcHJpY2UYFCABKAUSDAoEc2l6ZRgeIAEoBRIVCg1vcmRlcmZpbGxoZWFkGCggASgMEgwKBHByZXYYMiABKAwitgEKCU9yZGVyTWV0YRIOCgZyZWZudW0YASABKAUSDQoFZm5hbWUYCiABKAkSEAoIcGxheWVyaWQYFCABKAkSDwoHYnV5c2lkZRgeIAEoCBIMCgRzaXplGCggASgFEg0KBXByaWNlGDIgASgFEhAKCHR4bWV0YWlkGDwgASgMEhcKD29yZGVyZmlsbG1ldGFpZBhGIAEoDBIRCgl0aW1lc3RhbXAYSyABKAUSDAoEcHJldhhQIAEoDCKpAQoNT3JkZXJGaWxsTWV0YRIOCgZyZWZudW0YASABKAUSDQoFZm5hbWUYCiABKAkSEAoIcGxheWVyaWQYDyABKAkSEQoJZmlsbHByaWNlGBQgASgFEhAKCGZpbGxzaXplGB4gASgFEg8KB2J1eXNpZGUYIyABKAgSEQoJdGltZXN0YW1wGCggASgFEhAKCHR4bWV0YWlkGDwgASgMEgwKBHByZXYYRiABKAwibQoPR2xvYmFsU3RhdGVNZXRhEiwKC2dsb2JhbHN0YXRlGAogASgLMhcuZmFudGFzeWJpdC5HbG9iYWxTdGF0ZRIQCgh0cm1ldGFpZBgUIAEoDBIMCgRwcmV2GB4gASgMEgwKBG5leHQYKCABKAwingEKClBsYXllck1ldGESEAoIcGxheWVyaWQYCiABKAkSKwoLcGxheWVyX2Jhc2UYFCABKAsyFi5mYW50YXN5Yml0LlBsYXllckJhc2USLwoNcGxheWVyX3N0YXR1cxgeIAEoCzIYLmZhbnRhc3liaXQuUGxheWVyU3RhdHVzEhIKCmRhdGFtZXRhaWQYKCABKAwSDAoEcHJldhgyIAEoDCJCCghUZWFtTWV0YRIOCgZ0ZWFtaWQYCiABKAkSGAoQcGxheWVybWV0YWlkcm9vdBgUIAEoDBIMCgRwcmV2GB4gASgMIt0BCgdwYnN0YXRlEhcKD3NjaGVkdWxlc3RhdGVpZBgKIAEoDBIaChJsZWFkZXJib2FyZHN0YXRlaWQYFCABKAwSEwoLcHJvanN0YXRlaWQYHiABKAwSEgoKcG9zc3RhdGVpZBgjIAEoDBIUCgxvcmRlcnN0YXRlaWQYKCABKAwSFQoNbWFya2V0c3RhdGVpZBgyIAEoDBIVCg1nbG9iYWxzdGF0ZWlkGDwgASgMEhQKDHRlYW1zdGF0ZW1pZBhGIAEoDBIaChJmYW50YXN5bmFtZXN0YXRlaWQYUCABKAwiKgoKTWVya2xlVHJlZRIMCgRyb290GAogASgMEg4KBmxlYXZlcxgUIAMoDA==', 4033);
    }
    if (!this._desc)
      console.warn('Failed to initialize: StateData.proto');
    return this._desc;
  },
};

var BlockMeta = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof BlockMeta) {
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
      this.setBlocknum(rawArray[0][0]);
      this.setTrmetaid(rawArray[0][1]);
      this.setTxmetaroot(rawArray[0][2]);
      this.setPbstateid(rawArray[0][3]);
      this.setPrev(rawArray[0][4]);
      this.setNext(rawArray[0][5]);
    }
  };

  type.prototype.getBlocknum = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setBlocknum = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearBlocknum = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'blocknum', {
    get: type.prototype.getBlocknum,
    set: type.prototype.setBlocknum,
  });

  type.prototype.getTrmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setTrmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearTrmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'trmetaid', {
    get: type.prototype.getTrmetaid,
    set: type.prototype.setTrmetaid,
  });

  type.prototype.getTxmetaroot = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setTxmetaroot = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearTxmetaroot = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaroot', {
    get: type.prototype.getTxmetaroot,
    set: type.prototype.setTxmetaroot,
  });

  type.prototype.getPbstateid = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setPbstateid = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPbstateid = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'pbstateid', {
    get: type.prototype.getPbstateid,
    set: type.prototype.setPbstateid,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setPrev = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  type.prototype.getNext = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setNext = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearNext = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'next', {
    get: type.prototype.getNext,
    set: type.prototype.setNext,
  });

  return type;
})();

var TxMeta = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TxMeta) {
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
      this.setBlocknum(rawArray[0][0]);
      this.setTxnum(rawArray[0][1]);
      this.setTxtype(rawArray[0][2]);
      if (typeof this.tx == 'undefined') {
        this.setTx({});
      }
      this._tx._mergeFromRawArray(rawArray[0][3]);
      this.setFantasyName(rawArray[0][4]);
      this.setId(rawArray[0][5]);
    }
  };

  type.prototype.getBlocknum = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setBlocknum = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearBlocknum = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'blocknum', {
    get: type.prototype.getBlocknum,
    set: type.prototype.setBlocknum,
  });

  type.prototype.getTxnum = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setTxnum = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearTxnum = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'txnum', {
    get: type.prototype.getTxnum,
    set: type.prototype.setTxnum,
  });

  type.prototype.getTxtype = function() {
    return typeof this._fields[2] == 'undefined' ? TransType.NAME : this._fields[2];
  }
  type.prototype.setTxtype = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearTxtype = function() {
    this._fields[2] = TransType.NAME;
  };
  Object.defineProperty(type.prototype, 'txtype', {
    get: type.prototype.getTxtype,
    set: type.prototype.setTxtype,
  });

  type.prototype.getTx = function() {
    return this._tx;
  }
  type.prototype.setTx = function(value) {
    var msg = this._maybeConvertToMessage(Q__ProtoData__.Transaction, value);
    this._tx = msg;
    this._fields[3] = msg._raw;
  };
  type.prototype.clearTx = function() {
    this._fields[3] = undefined;
    this._tx = undefined;
  };
  Object.defineProperty(type.prototype, 'tx', {
    get: type.prototype.getTx,
    set: type.prototype.setTx,
  });

  type.prototype.getFantasyName = function() {
    return typeof this._fields[4] == 'undefined' ? '' : this._fields[4];
  }
  type.prototype.setFantasyName = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearFantasyName = function() {
    this._fields[4] = '';
  };
  Object.defineProperty(type.prototype, 'fantasyName', {
    get: type.prototype.getFantasyName,
    set: type.prototype.setFantasyName,
  });

  type.prototype.getId = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setId = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearId = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'id', {
    get: type.prototype.getId,
    set: type.prototype.setId,
  });

  return type;
})();

var TrMeta = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TrMeta) {
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
      this.setBlocknum(rawArray[0][0]);
      this.setType(rawArray[0][1]);
      this.setSeason(rawArray[0][2]);
      this.setWeek(rawArray[0][3]);
      this.setGamemetaroot(rawArray[0][4]);
      this.setDatametaroot(rawArray[0][5]);
      this.setTxmetaid(rawArray[0][6]);
    }
  };

  type.prototype.getBlocknum = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setBlocknum = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearBlocknum = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'blocknum', {
    get: type.prototype.getBlocknum,
    set: type.prototype.setBlocknum,
  });

  type.prototype.getType = function() {
    return typeof this._fields[1] == 'undefined' ? TrType.SEASONSTART : this._fields[1];
  }
  type.prototype.setType = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearType = function() {
    this._fields[1] = TrType.SEASONSTART;
  };
  Object.defineProperty(type.prototype, 'type', {
    get: type.prototype.getType,
    set: type.prototype.setType,
  });

  type.prototype.getSeason = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setSeason = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearSeason = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'season', {
    get: type.prototype.getSeason,
    set: type.prototype.setSeason,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setWeek = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getGamemetaroot = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setGamemetaroot = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearGamemetaroot = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamemetaroot', {
    get: type.prototype.getGamemetaroot,
    set: type.prototype.setGamemetaroot,
  });

  type.prototype.getDatametaroot = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setDatametaroot = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearDatametaroot = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'datametaroot', {
    get: type.prototype.getDatametaroot,
    set: type.prototype.setDatametaroot,
  });

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[6] == 'undefined' ?  new ArrayBuffer() : this._fields[6];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[6] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  return type;
})();

var GameMeta = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GameMeta) {
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
      this.setTxmetaid(rawArray[0][0]);
      if (typeof this.gamedata == 'undefined') {
        this.setGamedata({});
      }
      this._gamedata._mergeFromRawArray(rawArray[0][1]);
    }
  };

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[0] == 'undefined' ?  new ArrayBuffer() : this._fields[0];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[0] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  type.prototype.getGamedata = function() {
    return this._gamedata;
  }
  type.prototype.setGamedata = function(value) {
    var msg = this._maybeConvertToMessage(Q__ProtoData__.GameData, value);
    this._gamedata = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearGamedata = function() {
    this._fields[1] = undefined;
    this._gamedata = undefined;
  };
  Object.defineProperty(type.prototype, 'gamedata', {
    get: type.prototype.getGamedata,
    set: type.prototype.setGamedata,
  });

  return type;
})();

var DataMeta = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof DataMeta) {
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
      this.setTxmetaid(rawArray[0][0]);
      if (typeof this.data == 'undefined') {
        this.setData({});
      }
      this._data._mergeFromRawArray(rawArray[0][1]);
    }
  };

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[0] == 'undefined' ?  new ArrayBuffer() : this._fields[0];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[0] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  type.prototype.getData = function() {
    return this._data;
  }
  type.prototype.setData = function(value) {
    var msg = this._maybeConvertToMessage(Q__ProtoData__.Data, value);
    this._data = msg;
    this._fields[1] = msg._raw;
  };
  type.prototype.clearData = function() {
    this._fields[1] = undefined;
    this._data = undefined;
  };
  Object.defineProperty(type.prototype, 'data', {
    get: type.prototype.getData,
    set: type.prototype.setData,
  });

  return type;
})();

var LeaderBoardMeta = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof LeaderBoardMeta) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(5));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setWeek(rawArray[0][0]);
      this.setFnbalmetaroot(rawArray[0][1]);
      this.setPrev(rawArray[0][2]);
      this.setNext(rawArray[0][3]);
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

  type.prototype.getFnbalmetaroot = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setFnbalmetaroot = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearFnbalmetaroot = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'fnbalmetaroot', {
    get: type.prototype.getFnbalmetaroot,
    set: type.prototype.setFnbalmetaroot,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setPrev = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  type.prototype.getNext = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setNext = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearNext = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'next', {
    get: type.prototype.getNext,
    set: type.prototype.setNext,
  });

  return type;
})();

var FantasyNameBalMeta = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyNameBalMeta) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(6));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setTxmetaid(rawArray[0][0]);
      this.setTrmetaid(rawArray[0][1]);
      if (typeof this.fnamebal == 'undefined') {
        this.setFnamebal({});
      }
      this._fnamebal._mergeFromRawArray(rawArray[0][2]);
      this.setAwardmetaroot(rawArray[0][3]);
      this.setPnlmetaroot(rawArray[0][4]);
      this.setPrev(rawArray[0][5]);
      this.setNext(rawArray[0][6]);
    }
  };

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[0] == 'undefined' ?  new ArrayBuffer() : this._fields[0];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[0] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  type.prototype.getTrmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setTrmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearTrmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'trmetaid', {
    get: type.prototype.getTrmetaid,
    set: type.prototype.setTrmetaid,
  });

  type.prototype.getFnamebal = function() {
    return this._fnamebal;
  }
  type.prototype.setFnamebal = function(value) {
    var msg = this._maybeConvertToMessage(Q__NameData__.FantasyNameBal, value);
    this._fnamebal = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearFnamebal = function() {
    this._fields[2] = undefined;
    this._fnamebal = undefined;
  };
  Object.defineProperty(type.prototype, 'fnamebal', {
    get: type.prototype.getFnamebal,
    set: type.prototype.setFnamebal,
  });

  type.prototype.getAwardmetaroot = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setAwardmetaroot = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearAwardmetaroot = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'awardmetaroot', {
    get: type.prototype.getAwardmetaroot,
    set: type.prototype.setAwardmetaroot,
  });

  type.prototype.getPnlmetaroot = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setPnlmetaroot = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearPnlmetaroot = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'pnlmetaroot', {
    get: type.prototype.getPnlmetaroot,
    set: type.prototype.setPnlmetaroot,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setPrev = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  type.prototype.getNext = function() {
    return typeof this._fields[6] == 'undefined' ?  new ArrayBuffer() : this._fields[6];
  }
  type.prototype.setNext = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearNext = function() {
    this._fields[6] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'next', {
    get: type.prototype.getNext,
    set: type.prototype.setNext,
  });

  return type;
})();

var FantasyNameState = (function() {
  var type = function(values) {
    this._fields = new Array(8);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof FantasyNameState) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(7));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setName(rawArray[0][0]);
      this.setFantasynamebalmetaid(rawArray[0][1]);
      this.setPosmetaroot(rawArray[0][2]);
      this.setOrdersmetaroot(rawArray[0][3]);
      this.setProjmetaroot(rawArray[0][4]);
      this.setPrev(rawArray[0][5]);
      this.setNext(rawArray[0][6]);
      this.setTxmetaid(rawArray[0][7]);
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

  type.prototype.getFantasynamebalmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setFantasynamebalmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearFantasynamebalmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'fantasynamebalmetaid', {
    get: type.prototype.getFantasynamebalmetaid,
    set: type.prototype.setFantasynamebalmetaid,
  });

  type.prototype.getPosmetaroot = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setPosmetaroot = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPosmetaroot = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'posmetaroot', {
    get: type.prototype.getPosmetaroot,
    set: type.prototype.setPosmetaroot,
  });

  type.prototype.getOrdersmetaroot = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setOrdersmetaroot = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearOrdersmetaroot = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'ordersmetaroot', {
    get: type.prototype.getOrdersmetaroot,
    set: type.prototype.setOrdersmetaroot,
  });

  type.prototype.getProjmetaroot = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setProjmetaroot = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearProjmetaroot = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'projmetaroot', {
    get: type.prototype.getProjmetaroot,
    set: type.prototype.setProjmetaroot,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setPrev = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  type.prototype.getNext = function() {
    return typeof this._fields[6] == 'undefined' ?  new ArrayBuffer() : this._fields[6];
  }
  type.prototype.setNext = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearNext = function() {
    this._fields[6] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'next', {
    get: type.prototype.getNext,
    set: type.prototype.setNext,
  });

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[7] == 'undefined' ?  new ArrayBuffer() : this._fields[7];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[7] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  return type;
})();

var WeekGameStatusMeta = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof WeekGameStatusMeta) {
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
      this.setWeek(rawArray[0][0]);
      this.setGameresultmetaroot(rawArray[0][1]);
      this.setIngameprojmetaroot(rawArray[0][2]);
      this.setOpengamestatusroot(rawArray[0][3]);
      this.setPrev(rawArray[0][4]);
      this.setNext(rawArray[0][5]);
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

  type.prototype.getGameresultmetaroot = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setGameresultmetaroot = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearGameresultmetaroot = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gameresultmetaroot', {
    get: type.prototype.getGameresultmetaroot,
    set: type.prototype.setGameresultmetaroot,
  });

  type.prototype.getIngameprojmetaroot = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setIngameprojmetaroot = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearIngameprojmetaroot = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'ingameprojmetaroot', {
    get: type.prototype.getIngameprojmetaroot,
    set: type.prototype.setIngameprojmetaroot,
  });

  type.prototype.getOpengamestatusroot = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setOpengamestatusroot = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearOpengamestatusroot = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'opengamestatusroot', {
    get: type.prototype.getOpengamestatusroot,
    set: type.prototype.setOpengamestatusroot,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setPrev = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  type.prototype.getNext = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setNext = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearNext = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'next', {
    get: type.prototype.getNext,
    set: type.prototype.setNext,
  });

  return type;
})();

var InGameProjMeta = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof InGameProjMeta) {
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
      this.setGameid(rawArray[0][0]);
      this.setGamestatusmetaid(rawArray[0][1]);
      this.setHomeprojmeta(rawArray[0][2]);
      this.setAwayprojmeta(rawArray[0][3]);
      this.setGamedatametaid(rawArray[0][4]);
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

  type.prototype.getGamestatusmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setGamestatusmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearGamestatusmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamestatusmetaid', {
    get: type.prototype.getGamestatusmetaid,
    set: type.prototype.setGamestatusmetaid,
  });

  type.prototype.getHomeprojmeta = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setHomeprojmeta = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearHomeprojmeta = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'homeprojmeta', {
    get: type.prototype.getHomeprojmeta,
    set: type.prototype.setHomeprojmeta,
  });

  type.prototype.getAwayprojmeta = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setAwayprojmeta = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearAwayprojmeta = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'awayprojmeta', {
    get: type.prototype.getAwayprojmeta,
    set: type.prototype.setAwayprojmeta,
  });

  type.prototype.getGamedatametaid = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setGamedatametaid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearGamedatametaid = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamedatametaid', {
    get: type.prototype.getGamedatametaid,
    set: type.prototype.setGamedatametaid,
  });

  return type;
})();

var GameResultsMeta = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GameResultsMeta) {
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
      this.setGameid(rawArray[0][0]);
      this.setGamestatusmetaid(rawArray[0][1]);
      this.setHomeresultmeta(rawArray[0][2]);
      this.setAwayresultmeta(rawArray[0][3]);
      this.setResultdatametaid(rawArray[0][4]);
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

  type.prototype.getGamestatusmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setGamestatusmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearGamestatusmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamestatusmetaid', {
    get: type.prototype.getGamestatusmetaid,
    set: type.prototype.setGamestatusmetaid,
  });

  type.prototype.getHomeresultmeta = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setHomeresultmeta = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearHomeresultmeta = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'homeresultmeta', {
    get: type.prototype.getHomeresultmeta,
    set: type.prototype.setHomeresultmeta,
  });

  type.prototype.getAwayresultmeta = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setAwayresultmeta = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearAwayresultmeta = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'awayresultmeta', {
    get: type.prototype.getAwayresultmeta,
    set: type.prototype.setAwayresultmeta,
  });

  type.prototype.getResultdatametaid = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setResultdatametaid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearResultdatametaid = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'resultdatametaid', {
    get: type.prototype.getResultdatametaid,
    set: type.prototype.setResultdatametaid,
  });

  return type;
})();

var GameStatusMeta = (function() {
  var type = function(values) {
    this._fields = new Array(8);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GameStatusMeta) {
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
      this.setId(rawArray[0][0]);
      this.setWeek(rawArray[0][1]);
      if (typeof this.gameinfo == 'undefined') {
        this.setGameinfo({});
      }
      this._gameinfo._mergeFromRawArray(rawArray[0][2]);
      if (typeof this.gamesatus == 'undefined') {
        this.setGamesatus({});
      }
      this._gamesatus._mergeFromRawArray(rawArray[0][3]);
      this.setGamemetaid(rawArray[0][4]);
      this.setDatametaid(rawArray[0][5]);
      this.setPrev(rawArray[0][6]);
      this.setNext(rawArray[0][7]);
    }
  };

  type.prototype.getId = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setId = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearId = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'id', {
    get: type.prototype.getId,
    set: type.prototype.setId,
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

  type.prototype.getGameinfo = function() {
    return this._gameinfo;
  }
  type.prototype.setGameinfo = function(value) {
    var msg = this._maybeConvertToMessage(Q__StaticData__.GameInfo, value);
    this._gameinfo = msg;
    this._fields[2] = msg._raw;
  };
  type.prototype.clearGameinfo = function() {
    this._fields[2] = undefined;
    this._gameinfo = undefined;
  };
  Object.defineProperty(type.prototype, 'gameinfo', {
    get: type.prototype.getGameinfo,
    set: type.prototype.setGameinfo,
  });

  type.prototype.getGamesatus = function() {
    return this._gamesatus;
  }
  type.prototype.setGamesatus = function(value) {
    var msg = this._maybeConvertToMessage(Q__StatusData__.GameStatus, value);
    this._gamesatus = msg;
    this._fields[3] = msg._raw;
  };
  type.prototype.clearGamesatus = function() {
    this._fields[3] = undefined;
    this._gamesatus = undefined;
  };
  Object.defineProperty(type.prototype, 'gamesatus', {
    get: type.prototype.getGamesatus,
    set: type.prototype.setGamesatus,
  });

  type.prototype.getGamemetaid = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setGamemetaid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearGamemetaid = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamemetaid', {
    get: type.prototype.getGamemetaid,
    set: type.prototype.setGamemetaid,
  });

  type.prototype.getDatametaid = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setDatametaid = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearDatametaid = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'datametaid', {
    get: type.prototype.getDatametaid,
    set: type.prototype.setDatametaid,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[6] == 'undefined' ?  new ArrayBuffer() : this._fields[6];
  }
  type.prototype.setPrev = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[6] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  type.prototype.getNext = function() {
    return typeof this._fields[7] == 'undefined' ?  new ArrayBuffer() : this._fields[7];
  }
  type.prototype.setNext = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearNext = function() {
    this._fields[7] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'next', {
    get: type.prototype.getNext,
    set: type.prototype.setNext,
  });

  return type;
})();

var TeamProjMeta = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TeamProjMeta) {
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
      this.setGameid(rawArray[0][0]);
      this.setTeam(rawArray[0][1]);
      this.setKickofftime(rawArray[0][2]);
      this.setWeek(rawArray[0][3]);
      this.setGameplayerprojmetaroot(rawArray[0][4]);
      this.setGamedatametaid(rawArray[0][5]);
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

  type.prototype.getTeam = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setTeam = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearTeam = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'team', {
    get: type.prototype.getTeam,
    set: type.prototype.setTeam,
  });

  type.prototype.getKickofftime = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setKickofftime = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearKickofftime = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'kickofftime', {
    get: type.prototype.getKickofftime,
    set: type.prototype.setKickofftime,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setWeek = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getGameplayerprojmetaroot = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setGameplayerprojmetaroot = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearGameplayerprojmetaroot = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gameplayerprojmetaroot', {
    get: type.prototype.getGameplayerprojmetaroot,
    set: type.prototype.setGameplayerprojmetaroot,
  });

  type.prototype.getGamedatametaid = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setGamedatametaid = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearGamedatametaid = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamedatametaid', {
    get: type.prototype.getGamedatametaid,
    set: type.prototype.setGamedatametaid,
  });

  return type;
})();

var TeamResultMeta = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TeamResultMeta) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(13));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setGameid(rawArray[0][0]);
      this.setTeam(rawArray[0][1]);
      this.setKickofftime(rawArray[0][2]);
      this.setWeek(rawArray[0][3]);
      this.setPlayerresultmetaroot(rawArray[0][4]);
      this.setResultdatametaid(rawArray[0][5]);
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

  type.prototype.getTeam = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setTeam = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearTeam = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'team', {
    get: type.prototype.getTeam,
    set: type.prototype.setTeam,
  });

  type.prototype.getKickofftime = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setKickofftime = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearKickofftime = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'kickofftime', {
    get: type.prototype.getKickofftime,
    set: type.prototype.setKickofftime,
  });

  type.prototype.getWeek = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setWeek = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearWeek = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'week', {
    get: type.prototype.getWeek,
    set: type.prototype.setWeek,
  });

  type.prototype.getPlayerresultmetaroot = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setPlayerresultmetaroot = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearPlayerresultmetaroot = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'playerresultmetaroot', {
    get: type.prototype.getPlayerresultmetaroot,
    set: type.prototype.setPlayerresultmetaroot,
  });

  type.prototype.getResultdatametaid = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setResultdatametaid = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearResultdatametaid = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'resultdatametaid', {
    get: type.prototype.getResultdatametaid,
    set: type.prototype.setResultdatametaid,
  });

  return type;
})();

var GamePlayerProjMeta = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GamePlayerProjMeta) {
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
      this.setPlayerid(rawArray[0][0]);
      this.setGamestatusmetaid(rawArray[0][1]);
      this.setProjmetaplayerroot(rawArray[0][2]);
      this.setPosmetaplayerroot(rawArray[0][3]);
      this.setGamedatametaid(rawArray[0][4]);
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

  type.prototype.getGamestatusmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setGamestatusmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearGamestatusmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamestatusmetaid', {
    get: type.prototype.getGamestatusmetaid,
    set: type.prototype.setGamestatusmetaid,
  });

  type.prototype.getProjmetaplayerroot = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setProjmetaplayerroot = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearProjmetaplayerroot = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'projmetaplayerroot', {
    get: type.prototype.getProjmetaplayerroot,
    set: type.prototype.setProjmetaplayerroot,
  });

  type.prototype.getPosmetaplayerroot = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setPosmetaplayerroot = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPosmetaplayerroot = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'posmetaplayerroot', {
    get: type.prototype.getPosmetaplayerroot,
    set: type.prototype.setPosmetaplayerroot,
  });

  type.prototype.getGamedatametaid = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setGamedatametaid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearGamedatametaid = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamedatametaid', {
    get: type.prototype.getGamedatametaid,
    set: type.prototype.setGamedatametaid,
  });

  return type;
})();

var ProjMeta = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof ProjMeta) {
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
      this.setName(rawArray[0][0]);
      this.setProj(rawArray[0][1]);
      this.setPlayerid(rawArray[0][2]);
      this.setTxmetaid(rawArray[0][3]);
      this.setPrev(rawArray[0][4]);
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

  type.prototype.getProj = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setProj = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearProj = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'proj', {
    get: type.prototype.getProj,
    set: type.prototype.setProj,
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

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setPrev = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  return type;
})();

var PosMeta = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PosMeta) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(16));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setName(rawArray[0][0]);
      this.setQty(rawArray[0][1]);
      this.setPrice(rawArray[0][2]);
      this.setPlayerid(rawArray[0][3]);
      this.setTxmetaid(rawArray[0][4]);
      this.setPrev(rawArray[0][5]);
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

  type.prototype.getQty = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setQty = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearQty = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'qty', {
    get: type.prototype.getQty,
    set: type.prototype.setQty,
  });

  type.prototype.getPrice = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setPrice = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  type.prototype.getPlayerid = function() {
    return typeof this._fields[3] == 'undefined' ? '' : this._fields[3];
  }
  type.prototype.setPlayerid = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPlayerid = function() {
    this._fields[3] = '';
  };
  Object.defineProperty(type.prototype, 'playerid', {
    get: type.prototype.getPlayerid,
    set: type.prototype.setPlayerid,
  });

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setPrev = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  return type;
})();

var PlayerResultMeta = (function() {
  var type = function(values) {
    this._fields = new Array(7);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PlayerResultMeta) {
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
      this.setPlayerid(rawArray[0][0]);
      this.setGamestatusmetaid(rawArray[0][1]);
      this.setResult(rawArray[0][2]);
      if (typeof this.stats == 'undefined') {
        this.setStats({});
      }
      this._stats._mergeFromRawArray(rawArray[0][3]);
      this.setAwardmetaplayerroot(rawArray[0][4]);
      this.setPnlmetaplayerroot(rawArray[0][5]);
      this.setResultdatametaid(rawArray[0][6]);
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

  type.prototype.getGamestatusmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setGamestatusmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearGamestatusmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'gamestatusmetaid', {
    get: type.prototype.getGamestatusmetaid,
    set: type.prototype.setGamestatusmetaid,
  });

  type.prototype.getResult = function() {
    return typeof this._fields[2] == 'undefined' ? 0.000000 : this._fields[2];
  }
  type.prototype.setResult = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearResult = function() {
    this._fields[2] = 0.000000;
  };
  Object.defineProperty(type.prototype, 'result', {
    get: type.prototype.getResult,
    set: type.prototype.setResult,
  });

  type.prototype.getStats = function() {
    return this._stats;
  }
  type.prototype.setStats = function(value) {
    var msg = this._maybeConvertToMessage(Q__StaticData__.Stats, value);
    this._stats = msg;
    this._fields[3] = msg._raw;
  };
  type.prototype.clearStats = function() {
    this._fields[3] = undefined;
    this._stats = undefined;
  };
  Object.defineProperty(type.prototype, 'stats', {
    get: type.prototype.getStats,
    set: type.prototype.setStats,
  });

  type.prototype.getAwardmetaplayerroot = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setAwardmetaplayerroot = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearAwardmetaplayerroot = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'awardmetaplayerroot', {
    get: type.prototype.getAwardmetaplayerroot,
    set: type.prototype.setAwardmetaplayerroot,
  });

  type.prototype.getPnlmetaplayerroot = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setPnlmetaplayerroot = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearPnlmetaplayerroot = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'pnlmetaplayerroot', {
    get: type.prototype.getPnlmetaplayerroot,
    set: type.prototype.setPnlmetaplayerroot,
  });

  type.prototype.getResultdatametaid = function() {
    return typeof this._fields[6] == 'undefined' ?  new ArrayBuffer() : this._fields[6];
  }
  type.prototype.setResultdatametaid = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearResultdatametaid = function() {
    this._fields[6] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'resultdatametaid', {
    get: type.prototype.getResultdatametaid,
    set: type.prototype.setResultdatametaid,
  });

  return type;
})();

var AwardMeta = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof AwardMeta) {
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
      this.setName(rawArray[0][0]);
      this.setProj(rawArray[0][1]);
      this.setAward(rawArray[0][2]);
      this.setProjmetaid(rawArray[0][3]);
      this.setResultdatametaid(rawArray[0][4]);
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

  type.prototype.getProj = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setProj = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearProj = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'proj', {
    get: type.prototype.getProj,
    set: type.prototype.setProj,
  });

  type.prototype.getAward = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setAward = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearAward = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'award', {
    get: type.prototype.getAward,
    set: type.prototype.setAward,
  });

  type.prototype.getProjmetaid = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setProjmetaid = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearProjmetaid = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'projmetaid', {
    get: type.prototype.getProjmetaid,
    set: type.prototype.setProjmetaid,
  });

  type.prototype.getResultdatametaid = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setResultdatametaid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearResultdatametaid = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'resultdatametaid', {
    get: type.prototype.getResultdatametaid,
    set: type.prototype.setResultdatametaid,
  });

  return type;
})();

var PnlMeta = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PnlMeta) {
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
      if (typeof this.fantaybitpnl == 'undefined') {
        this.setFantaybitpnl({});
      }
      this._fantaybitpnl._mergeFromRawArray(rawArray[0][0]);
      this.setProjmetaid(rawArray[0][1]);
      this.setResultdatametaid(rawArray[0][2]);
    }
  };

  type.prototype.getFantaybitpnl = function() {
    return this._fantaybitpnl;
  }
  type.prototype.setFantaybitpnl = function(value) {
    var msg = this._maybeConvertToMessage(Q__NameData__.FantasyBitPnl, value);
    this._fantaybitpnl = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearFantaybitpnl = function() {
    this._fields[0] = undefined;
    this._fantaybitpnl = undefined;
  };
  Object.defineProperty(type.prototype, 'fantaybitpnl', {
    get: type.prototype.getFantaybitpnl,
    set: type.prototype.setFantaybitpnl,
  });

  type.prototype.getProjmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setProjmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearProjmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'projmetaid', {
    get: type.prototype.getProjmetaid,
    set: type.prototype.setProjmetaid,
  });

  type.prototype.getResultdatametaid = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setResultdatametaid = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearResultdatametaid = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'resultdatametaid', {
    get: type.prototype.getResultdatametaid,
    set: type.prototype.setResultdatametaid,
  });

  return type;
})();

var PlayerMarketState = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PlayerMarketState) {
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
      this.setLimitbookmetaid(rawArray[0][0]);
      this.setPlayerid(rawArray[0][1]);
      this.setMarketticmetaid(rawArray[0][2]);
      this.setPrev(rawArray[0][3]);
      this.setNext(rawArray[0][4]);
    }
  };

  type.prototype.getLimitbookmetaid = function() {
    return typeof this._fields[0] == 'undefined' ?  new ArrayBuffer() : this._fields[0];
  }
  type.prototype.setLimitbookmetaid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearLimitbookmetaid = function() {
    this._fields[0] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'limitbookmetaid', {
    get: type.prototype.getLimitbookmetaid,
    set: type.prototype.setLimitbookmetaid,
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

  type.prototype.getMarketticmetaid = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setMarketticmetaid = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearMarketticmetaid = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'marketticmetaid', {
    get: type.prototype.getMarketticmetaid,
    set: type.prototype.setMarketticmetaid,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setPrev = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  type.prototype.getNext = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setNext = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearNext = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'next', {
    get: type.prototype.getNext,
    set: type.prototype.setNext,
  });

  return type;
})();

var LimitBookMeta = (function() {
  var type = function(values) {
    this._fields = new Array(6);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof LimitBookMeta) {
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
      this.setBid(rawArray[0][0]);
      this.setBidsize(rawArray[0][1]);
      this.setAsksize(rawArray[0][2]);
      this.setAsk(rawArray[0][3]);
      this.setBidordermetaroot(rawArray[0][4]);
      this.setAskordermetaroot(rawArray[0][5]);
    }
  };

  type.prototype.getBid = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setBid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearBid = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'bid', {
    get: type.prototype.getBid,
    set: type.prototype.setBid,
  });

  type.prototype.getBidsize = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setBidsize = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearBidsize = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'bidsize', {
    get: type.prototype.getBidsize,
    set: type.prototype.setBidsize,
  });

  type.prototype.getAsksize = function() {
    return typeof this._fields[2] == 'undefined' ? 0 : this._fields[2];
  }
  type.prototype.setAsksize = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearAsksize = function() {
    this._fields[2] = 0;
  };
  Object.defineProperty(type.prototype, 'asksize', {
    get: type.prototype.getAsksize,
    set: type.prototype.setAsksize,
  });

  type.prototype.getAsk = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setAsk = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearAsk = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'ask', {
    get: type.prototype.getAsk,
    set: type.prototype.setAsk,
  });

  type.prototype.getBidordermetaroot = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setBidordermetaroot = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearBidordermetaroot = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'bidordermetaroot', {
    get: type.prototype.getBidordermetaroot,
    set: type.prototype.setBidordermetaroot,
  });

  type.prototype.getAskordermetaroot = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setAskordermetaroot = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearAskordermetaroot = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'askordermetaroot', {
    get: type.prototype.getAskordermetaroot,
    set: type.prototype.setAskordermetaroot,
  });

  return type;
})();

var MarketTicMeta = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof MarketTicMeta) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(22));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setPrice(rawArray[0][0]);
      this.setSize(rawArray[0][1]);
      this.setOrderfillhead(rawArray[0][2]);
      this.setPrev(rawArray[0][3]);
    }
  };

  type.prototype.getPrice = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setPrice = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearPrice = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'price', {
    get: type.prototype.getPrice,
    set: type.prototype.setPrice,
  });

  type.prototype.getSize = function() {
    return typeof this._fields[1] == 'undefined' ? 0 : this._fields[1];
  }
  type.prototype.setSize = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearSize = function() {
    this._fields[1] = 0;
  };
  Object.defineProperty(type.prototype, 'size', {
    get: type.prototype.getSize,
    set: type.prototype.setSize,
  });

  type.prototype.getOrderfillhead = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setOrderfillhead = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearOrderfillhead = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'orderfillhead', {
    get: type.prototype.getOrderfillhead,
    set: type.prototype.setOrderfillhead,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setPrev = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  return type;
})();

var OrderMeta = (function() {
  var type = function(values) {
    this._fields = new Array(10);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof OrderMeta) {
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
      this.setRefnum(rawArray[0][0]);
      this.setFname(rawArray[0][1]);
      this.setPlayerid(rawArray[0][2]);
      this.setBuyside(rawArray[0][3]);
      this.setSize(rawArray[0][4]);
      this.setPrice(rawArray[0][5]);
      this.setTxmetaid(rawArray[0][6]);
      this.setOrderfillmetaid(rawArray[0][7]);
      this.setTimestamp(rawArray[0][8]);
      this.setPrev(rawArray[0][9]);
    }
  };

  type.prototype.getRefnum = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setRefnum = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearRefnum = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'refnum', {
    get: type.prototype.getRefnum,
    set: type.prototype.setRefnum,
  });

  type.prototype.getFname = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setFname = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearFname = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'fname', {
    get: type.prototype.getFname,
    set: type.prototype.setFname,
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

  type.prototype.getSize = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setSize = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearSize = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'size', {
    get: type.prototype.getSize,
    set: type.prototype.setSize,
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

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[6] == 'undefined' ?  new ArrayBuffer() : this._fields[6];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[6] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  type.prototype.getOrderfillmetaid = function() {
    return typeof this._fields[7] == 'undefined' ?  new ArrayBuffer() : this._fields[7];
  }
  type.prototype.setOrderfillmetaid = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearOrderfillmetaid = function() {
    this._fields[7] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'orderfillmetaid', {
    get: type.prototype.getOrderfillmetaid,
    set: type.prototype.setOrderfillmetaid,
  });

  type.prototype.getTimestamp = function() {
    return typeof this._fields[8] == 'undefined' ? 0 : this._fields[8];
  }
  type.prototype.setTimestamp = function(value) {
      this._fields[8] = value;
  };
  type.prototype.clearTimestamp = function() {
    this._fields[8] = 0;
  };
  Object.defineProperty(type.prototype, 'timestamp', {
    get: type.prototype.getTimestamp,
    set: type.prototype.setTimestamp,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[9] == 'undefined' ?  new ArrayBuffer() : this._fields[9];
  }
  type.prototype.setPrev = function(value) {
      this._fields[9] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[9] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  return type;
})();

var OrderFillMeta = (function() {
  var type = function(values) {
    this._fields = new Array(9);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof OrderFillMeta) {
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
      this.setRefnum(rawArray[0][0]);
      this.setFname(rawArray[0][1]);
      this.setPlayerid(rawArray[0][2]);
      this.setFillprice(rawArray[0][3]);
      this.setFillsize(rawArray[0][4]);
      this.setBuyside(rawArray[0][5]);
      this.setTimestamp(rawArray[0][6]);
      this.setTxmetaid(rawArray[0][7]);
      this.setPrev(rawArray[0][8]);
    }
  };

  type.prototype.getRefnum = function() {
    return typeof this._fields[0] == 'undefined' ? 0 : this._fields[0];
  }
  type.prototype.setRefnum = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearRefnum = function() {
    this._fields[0] = 0;
  };
  Object.defineProperty(type.prototype, 'refnum', {
    get: type.prototype.getRefnum,
    set: type.prototype.setRefnum,
  });

  type.prototype.getFname = function() {
    return typeof this._fields[1] == 'undefined' ? '' : this._fields[1];
  }
  type.prototype.setFname = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearFname = function() {
    this._fields[1] = '';
  };
  Object.defineProperty(type.prototype, 'fname', {
    get: type.prototype.getFname,
    set: type.prototype.setFname,
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

  type.prototype.getFillprice = function() {
    return typeof this._fields[3] == 'undefined' ? 0 : this._fields[3];
  }
  type.prototype.setFillprice = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearFillprice = function() {
    this._fields[3] = 0;
  };
  Object.defineProperty(type.prototype, 'fillprice', {
    get: type.prototype.getFillprice,
    set: type.prototype.setFillprice,
  });

  type.prototype.getFillsize = function() {
    return typeof this._fields[4] == 'undefined' ? 0 : this._fields[4];
  }
  type.prototype.setFillsize = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearFillsize = function() {
    this._fields[4] = 0;
  };
  Object.defineProperty(type.prototype, 'fillsize', {
    get: type.prototype.getFillsize,
    set: type.prototype.setFillsize,
  });

  type.prototype.getBuyside = function() {
    return typeof this._fields[5] == 'undefined' ? false : this._fields[5];
  }
  type.prototype.setBuyside = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearBuyside = function() {
    this._fields[5] = false;
  };
  Object.defineProperty(type.prototype, 'buyside', {
    get: type.prototype.getBuyside,
    set: type.prototype.setBuyside,
  });

  type.prototype.getTimestamp = function() {
    return typeof this._fields[6] == 'undefined' ? 0 : this._fields[6];
  }
  type.prototype.setTimestamp = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearTimestamp = function() {
    this._fields[6] = 0;
  };
  Object.defineProperty(type.prototype, 'timestamp', {
    get: type.prototype.getTimestamp,
    set: type.prototype.setTimestamp,
  });

  type.prototype.getTxmetaid = function() {
    return typeof this._fields[7] == 'undefined' ?  new ArrayBuffer() : this._fields[7];
  }
  type.prototype.setTxmetaid = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearTxmetaid = function() {
    this._fields[7] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'txmetaid', {
    get: type.prototype.getTxmetaid,
    set: type.prototype.setTxmetaid,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[8] == 'undefined' ?  new ArrayBuffer() : this._fields[8];
  }
  type.prototype.setPrev = function(value) {
      this._fields[8] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[8] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  return type;
})();

var GlobalStateMeta = (function() {
  var type = function(values) {
    this._fields = new Array(4);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof GlobalStateMeta) {
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
      if (typeof this.globalstate == 'undefined') {
        this.setGlobalstate({});
      }
      this._globalstate._mergeFromRawArray(rawArray[0][0]);
      this.setTrmetaid(rawArray[0][1]);
      this.setPrev(rawArray[0][2]);
      this.setNext(rawArray[0][3]);
    }
  };

  type.prototype.getGlobalstate = function() {
    return this._globalstate;
  }
  type.prototype.setGlobalstate = function(value) {
    var msg = this._maybeConvertToMessage(Q__StatusData__.GlobalState, value);
    this._globalstate = msg;
    this._fields[0] = msg._raw;
  };
  type.prototype.clearGlobalstate = function() {
    this._fields[0] = undefined;
    this._globalstate = undefined;
  };
  Object.defineProperty(type.prototype, 'globalstate', {
    get: type.prototype.getGlobalstate,
    set: type.prototype.setGlobalstate,
  });

  type.prototype.getTrmetaid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setTrmetaid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearTrmetaid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'trmetaid', {
    get: type.prototype.getTrmetaid,
    set: type.prototype.setTrmetaid,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setPrev = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  type.prototype.getNext = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setNext = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearNext = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'next', {
    get: type.prototype.getNext,
    set: type.prototype.setNext,
  });

  return type;
})();

var PlayerMeta = (function() {
  var type = function(values) {
    this._fields = new Array(5);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof PlayerMeta) {
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
      this.setPlayerid(rawArray[0][0]);
      if (typeof this.playerBase == 'undefined') {
        this.setPlayerBase({});
      }
      this._playerBase._mergeFromRawArray(rawArray[0][1]);
      if (typeof this.playerStatus == 'undefined') {
        this.setPlayerStatus({});
      }
      this._playerStatus._mergeFromRawArray(rawArray[0][2]);
      this.setDatametaid(rawArray[0][3]);
      this.setPrev(rawArray[0][4]);
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

  type.prototype.getDatametaid = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setDatametaid = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearDatametaid = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'datametaid', {
    get: type.prototype.getDatametaid,
    set: type.prototype.setDatametaid,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setPrev = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  return type;
})();

var TeamMeta = (function() {
  var type = function(values) {
    this._fields = new Array(3);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof TeamMeta) {
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
      this.setTeamid(rawArray[0][0]);
      this.setPlayermetaidroot(rawArray[0][1]);
      this.setPrev(rawArray[0][2]);
    }
  };

  type.prototype.getTeamid = function() {
    return typeof this._fields[0] == 'undefined' ? '' : this._fields[0];
  }
  type.prototype.setTeamid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearTeamid = function() {
    this._fields[0] = '';
  };
  Object.defineProperty(type.prototype, 'teamid', {
    get: type.prototype.getTeamid,
    set: type.prototype.setTeamid,
  });

  type.prototype.getPlayermetaidroot = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setPlayermetaidroot = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearPlayermetaidroot = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'playermetaidroot', {
    get: type.prototype.getPlayermetaidroot,
    set: type.prototype.setPlayermetaidroot,
  });

  type.prototype.getPrev = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setPrev = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearPrev = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'prev', {
    get: type.prototype.getPrev,
    set: type.prototype.setPrev,
  });

  return type;
})();

var pbstate = (function() {
  var type = function(values) {
    this._fields = new Array(9);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
    if (values instanceof pbstate) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(28));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setSchedulestateid(rawArray[0][0]);
      this.setLeaderboardstateid(rawArray[0][1]);
      this.setProjstateid(rawArray[0][2]);
      this.setPosstateid(rawArray[0][3]);
      this.setOrderstateid(rawArray[0][4]);
      this.setMarketstateid(rawArray[0][5]);
      this.setGlobalstateid(rawArray[0][6]);
      this.setTeamstatemid(rawArray[0][7]);
      this.setFantasynamestateid(rawArray[0][8]);
    }
  };

  type.prototype.getSchedulestateid = function() {
    return typeof this._fields[0] == 'undefined' ?  new ArrayBuffer() : this._fields[0];
  }
  type.prototype.setSchedulestateid = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearSchedulestateid = function() {
    this._fields[0] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'schedulestateid', {
    get: type.prototype.getSchedulestateid,
    set: type.prototype.setSchedulestateid,
  });

  type.prototype.getLeaderboardstateid = function() {
    return typeof this._fields[1] == 'undefined' ?  new ArrayBuffer() : this._fields[1];
  }
  type.prototype.setLeaderboardstateid = function(value) {
      this._fields[1] = value;
  };
  type.prototype.clearLeaderboardstateid = function() {
    this._fields[1] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'leaderboardstateid', {
    get: type.prototype.getLeaderboardstateid,
    set: type.prototype.setLeaderboardstateid,
  });

  type.prototype.getProjstateid = function() {
    return typeof this._fields[2] == 'undefined' ?  new ArrayBuffer() : this._fields[2];
  }
  type.prototype.setProjstateid = function(value) {
      this._fields[2] = value;
  };
  type.prototype.clearProjstateid = function() {
    this._fields[2] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'projstateid', {
    get: type.prototype.getProjstateid,
    set: type.prototype.setProjstateid,
  });

  type.prototype.getPosstateid = function() {
    return typeof this._fields[3] == 'undefined' ?  new ArrayBuffer() : this._fields[3];
  }
  type.prototype.setPosstateid = function(value) {
      this._fields[3] = value;
  };
  type.prototype.clearPosstateid = function() {
    this._fields[3] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'posstateid', {
    get: type.prototype.getPosstateid,
    set: type.prototype.setPosstateid,
  });

  type.prototype.getOrderstateid = function() {
    return typeof this._fields[4] == 'undefined' ?  new ArrayBuffer() : this._fields[4];
  }
  type.prototype.setOrderstateid = function(value) {
      this._fields[4] = value;
  };
  type.prototype.clearOrderstateid = function() {
    this._fields[4] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'orderstateid', {
    get: type.prototype.getOrderstateid,
    set: type.prototype.setOrderstateid,
  });

  type.prototype.getMarketstateid = function() {
    return typeof this._fields[5] == 'undefined' ?  new ArrayBuffer() : this._fields[5];
  }
  type.prototype.setMarketstateid = function(value) {
      this._fields[5] = value;
  };
  type.prototype.clearMarketstateid = function() {
    this._fields[5] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'marketstateid', {
    get: type.prototype.getMarketstateid,
    set: type.prototype.setMarketstateid,
  });

  type.prototype.getGlobalstateid = function() {
    return typeof this._fields[6] == 'undefined' ?  new ArrayBuffer() : this._fields[6];
  }
  type.prototype.setGlobalstateid = function(value) {
      this._fields[6] = value;
  };
  type.prototype.clearGlobalstateid = function() {
    this._fields[6] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'globalstateid', {
    get: type.prototype.getGlobalstateid,
    set: type.prototype.setGlobalstateid,
  });

  type.prototype.getTeamstatemid = function() {
    return typeof this._fields[7] == 'undefined' ?  new ArrayBuffer() : this._fields[7];
  }
  type.prototype.setTeamstatemid = function(value) {
      this._fields[7] = value;
  };
  type.prototype.clearTeamstatemid = function() {
    this._fields[7] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'teamstatemid', {
    get: type.prototype.getTeamstatemid,
    set: type.prototype.setTeamstatemid,
  });

  type.prototype.getFantasynamestateid = function() {
    return typeof this._fields[8] == 'undefined' ?  new ArrayBuffer() : this._fields[8];
  }
  type.prototype.setFantasynamestateid = function(value) {
      this._fields[8] = value;
  };
  type.prototype.clearFantasynamestateid = function() {
    this._fields[8] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'fantasynamestateid', {
    get: type.prototype.getFantasynamestateid,
    set: type.prototype.setFantasynamestateid,
  });

  return type;
})();

var MerkleTree = (function() {
  var type = function(values) {
    this._fields = new Array(2);
    this._oneofs = new Uint32Array(0);
    this._raw = [this._fields, this._oneofs.buffer];
  this._fields[1] = new Array();
  if (values && values.leaves && values.leaves instanceof Array) {
    this.leaves(values.leaves);
  }
    if (values instanceof MerkleTree) {
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

  Protobuf.Message.createMessageType(type, _file.descriptor.messageType(29));

  type.prototype._mergeFromRawArray = function(rawArray) {
    if (rawArray && rawArray instanceof Array) {
      var oneofs = new Uint32Array(rawArray[1]);
      var field;
      this.setRoot(rawArray[0][0]);
      if (rawArray[0][1] instanceof Array) {
        this.leaves(rawArray[0][1]);
      }
    }
  };

  type.prototype.getRoot = function() {
    return typeof this._fields[0] == 'undefined' ?  new ArrayBuffer() : this._fields[0];
  }
  type.prototype.setRoot = function(value) {
      this._fields[0] = value;
  };
  type.prototype.clearRoot = function() {
    this._fields[0] =  new ArrayBuffer();
  };
  Object.defineProperty(type.prototype, 'root', {
    get: type.prototype.getRoot,
    set: type.prototype.setRoot,
  });

    // Replacement setter
  type.prototype.setLeaves = function(values) {
    if (!(values instanceof Array)) {
      throw new TypeError();
    }
      this._fields[1] = values.slice();
  };
  // Single value setter
  type.prototype.setLeavesAt = function(index, value) {
    this._fields[1][index] = value;
  };
  // Getter
  type.prototype.getLeavesAt = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    if(this._fields[1].length < index) {
      throw new RangeError();
    }
    return this._fields[1][index];
  };
  type.prototype.getLeavesAsArray = function() {
    return this._fields[1].slice();
  };
  type.prototype.leaves = function(indexOrValues, value) {
    if (typeof indexOrValues == 'undefined') {
      throw new TypeError('Not enough arguments');
    } else if (typeof indexOrValues == 'number') {
      if (typeof value == 'undefined') {
        return this.getLeavesAt(indexOrValues);
      } else {
        this.setLeavesAt(indexOrValues, value);
      }
    } else {
      this.setLeaves(indexOrValues);
    }
  };
  var leavesCount = function() {
    return this._fields[1].length;
  };
  Object.defineProperties(type.prototype, {
    leavesCount: { get: leavesCount },
    leavesSize: { get: leavesCount },
    leavesLength: { get: leavesCount },
  });
  type.prototype.getLeavesCount = leavesCount;
  type.prototype.getLeavesSize = leavesCount;
  type.prototype.getLeavesLength = leavesCount;
  type.prototype.addLeaves = function(value) {
    if (typeof value == 'undefined') {
      throw new TypeError('Cannot add undefined.');
    }
    this._fields[1].push(value);
  };
  type.prototype.resizeLeaves = function(len) {
    this._fields[1].length = values.length;
  };
  type.prototype.removeLeaves = function(index) {
    if (typeof index != 'number') {
      throw new TypeError('Index should be a number: ' + typeof index);
    }
    this._fields[1].splice(index, 1);
  };
  type.prototype.clearLeaves = function() {
    this._fields[1].length = 0;
  };
  return type;
})();

