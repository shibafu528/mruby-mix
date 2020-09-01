# -*- coding: utf-8 -*-

Plugin.create(:world) do
  # 登録されている全てのWorldを列挙する。
  # 次のような方法で、Enumeratorを取得できる。
  # ==== Example
  # collect(:worlds)
  defevent :worlds, prototype: [Pluggaloid::COLLECT]

  # 新たなアカウント _1_ を追加する (Host App上で追加された時に呼ぶ)
  defevent :world_create, prototype: [Diva::Model]

  # アカウント _1_ を削除する (Host App上で削除された時に呼ぶ)
  defevent :world_destroy, prototype: [Diva::Model]

  @world_slug_dict = {}         # world_slug(Symbol) => World URI(Diva::URI)

  def load_world
    Plugin::World::Gate.accounts.tap(&method(:check_world_uri))
  end

  def check_world_uri(new_worlds)
    new_worlds.each do |w|
      if @world_slug_dict.key?(w.slug)
        if @world_slug_dict[w.slug] != w.uri
          warn "The URI of World `#{w.slug}' is not defined. You must define a consistent URI for World Model. see: https://dev.mikutter.hachune.net/issues/1231"
        end
      else
        @world_slug_dict[w.slug] = w.uri
      end
    end
  end

  collection(:worlds) do |mutation|
    mutation.rewind { |_| load_world }

    on_world_create do |new|
      mutation.add(new)
      check_world_uri([new])
      Plugin.call(:world_after_created, new)
      Plugin.call(:service_registered, new) # 互換性のため
    end

    on_world_destroy do |target|
      mutation.delete(target)
      Plugin.call(:service_destroyed, target) # 互換性のため
    end
  end
end
