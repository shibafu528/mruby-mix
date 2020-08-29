module Mikutter::DivaHacks::ModelExtend
  # Modelの情報を設定する。
  # このメソッドを呼ぶと、他のプラグインがこのModelを見つけることができるようになるので、
  # 抽出タブの抽出条件が追加されたり、設定で背景色が指定できるようになる
  # ==== Args
  # [new_slug] Symbol
  # [name:] String Modelの名前
  # [reply:] bool このModelに、宛先が存在するなら真
  # [myself:] bool このModel、自分のアカウントによって作成できるなら真
  # [timeline:] bool 真ならタイムラインに表示することができる
  def register(new_slug,
               name: new_slug.to_s,
               reply: true,
               myself: true,
               timeline: false
              )
    @slug = new_slug.to_sym
    spec = @spec = Diva::ModelSpec.new(@slug,
                                            name.to_s.freeze,
                                            !!reply,
                                            !!myself,
                                            !!timeline
                                           ).freeze
    plugin do
      filter_retrievers do |retrievers|
        retrievers << spec
        [retrievers]
      end
    end
  end

  def plugin(&block)
    if not @slug
      raise Diva::DivaError, "`#{self}'.slug is not set."
    end
    Plugin.create(:"retriever_model_#{@slug}", &block)
  end

  # Entityクラスを設定する。
  # ==== Args
  # [klass] Class 新しく設定するEntityクラス
  # ==== Return
  # [Class] セットされた（されている）Entityクラス
  def entity_class(klass=nil)
    if klass
      @entity_class = klass
    else
      @entity_class ||= Diva::Entity::BlankEntity
    end
  end

  # あるURIが、このModelを示すものであれば真を返す条件 _condition_ を設定する。
  # _condition_ === uri が実行され、真を返せばそのURIをこのModelで取り扱えるということになる
  # ==== Args
  # [condition] 正規表現など、URIにマッチするもの
  # ==== Return
  # self
  # ==== Block
  # 実際にURIが指し示すリソースの内容を含んだModelを作って返す
  # ===== Args
  # [uri] URI マッチしたURI
  # ===== Return
  # [Delayer::Deferred::Deferredable]
  #   ネットワークアクセスを行って取得するなど取得に時間がかかる場合
  # [self]
  #   すぐにModelを生成できる場合、そのModel
  # ===== Raise
  # [Diva::ModelNotFoundError] _uri_ に対応するリソースが見つからなかった
  def handle(condition, &block)       # :yield: uri
    model_slug = self.slug
    plugin do
      if condition.is_a? Regexp
        filter_model_of_uri do |uri, models|
          if condition =~ uri.to_s
            models << model_slug
          end
          [uri, models]
        end
      else
        filter_model_of_uri do |uri, models|
          if condition === uri
            models << model_slug
          end
          [uri, models]
        end
      end
    end
    if block
      define_singleton_method(:find_by_uri, &block)
    end
  end

  # まだそのレコードのインスタンスがない場合、それを生成して返します。
  def new_ifnecessary(hash)
    type_strict hash => tcor(self, Hash)
    result_strict(self) do
      case hash
      when self
        hash
      when Hash
        self.new(hash)
      else
        raise ArgumentError.new("incorrect type #{hash.class} #{hash.inspect}")
      end
    end
  end

  def rewind(args)
    type_strict args => Hash
    result_strict(:merge){ new_ifnecessary(args) }.merge(args)
  end
end

module Diva
  class Model
    extend Mikutter::DivaHacks::ModelExtend
  end
end
